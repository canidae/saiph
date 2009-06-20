#include <stdlib.h>
#include "Debug.h"
#include "EventBus.h"
#include "Inventory.h"
#include "Item.h"
#include "Level.h"
#include "Saiph.h"
#include "World.h"

using namespace event;
using namespace std;

/* define static variables */
/* public */
bool Level::passable[UCHAR_MAX + 1] = {false};
/* private */
Point Level::pathing_queue[PATHING_QUEUE_SIZE] = {Point()};
unsigned char Level::uniquemap[UCHAR_MAX + 1][CHAR_MAX + 1] = {{0}};
int Level::pathcost[UCHAR_MAX + 1] = {0};
bool Level::dungeon[UCHAR_MAX + 1] = {false};
bool Level::monster[UCHAR_MAX + 1] = {false};
bool Level::item[UCHAR_MAX + 1] = {false};
bool Level::initialized = false;
ReceivedItems Level::received;
ItemsOnGround Level::on_ground;

/* constructors/destructor */
Level::Level(string name, int branch) : name(name), branch(branch), undiggable(false) {
	for (int a = 0; a < MAP_ROW_END + 1; ++a) {
		for (int b = 0; b < MAP_COL_END + 1; ++b) {
			dungeonmap[a][b] = SOLID_ROCK;
			monstermap[a][b] = ILLEGAL_MONSTER;
			searchmap[a][b] = 0;
		}
	}
	sscanf(name.c_str(), "%*[^0123456789]%d", &depth);
	if (!initialized)
		init();
}

/* methods */
void Level::analyze() {
	if (World::menu)
		return; // menu hides map, don't update
	if (World::engulfed) {
		 /* we'll still need to update monster's "visible" while engulfed,
		  * or she may attempt to farlook a monster she can't see */
		for (map<Point, Monster>::iterator m = monsters.begin(); m != monsters.end(); ++m)
			m->second.visible = false;
		return;
	}
	/* update changed symbols */
	for (vector<Point>::iterator c = World::changes.begin(); c != World::changes.end(); ++c)
		updateMapPoint(*c, (unsigned char) World::view[c->row][c->col], World::color[c->row][c->col]);
	/* update monsters */
	updateMonsters();
	/* update pathmap */
	updatePathMap();
	/* set point we're standing on "fully searched" */
	searchmap[Saiph::position.row][Saiph::position.col] = INT_MAX;
}

void Level::parseMessages(const string &messages) {
	/* parse messages that can help us find doors/staircases/etc. */
	string::size_type pos;
	if (messages.find(LEVEL_STAIRCASE_UP_HERE) != string::npos) {
		setDungeonSymbol(Saiph::position, STAIRS_UP);
	} else if (messages.find(LEVEL_STAIRCASE_DOWN_HERE) != string::npos) {
		setDungeonSymbol(Saiph::position, STAIRS_DOWN);
	} else if (messages.find(LEVEL_OPEN_DOOR_HERE) != string::npos) {
		setDungeonSymbol(Saiph::position, OPEN_DOOR);
	} else if (messages.find(LEVEL_GRAVE_HERE) != string::npos) {
		setDungeonSymbol(Saiph::position, GRAVE);
	} else if (messages.find(LEVEL_FOUNTAIN_HERE) != string::npos) {
		setDungeonSymbol(Saiph::position, FOUNTAIN);
	} else if (messages.find(LEVEL_FOUNTAIN_DRIES_UP) != string::npos || messages.find(LEVEL_FOUNTAIN_DRIES_UP2) != string::npos) {
		setDungeonSymbol(Saiph::position, FLOOR);
	} else if (messages.find(LEVEL_NO_STAIRS_DOWN_HERE) != string::npos || messages.find(LEVEL_NO_STAIRS_UP_HERE) != string::npos) {
		setDungeonSymbol(Saiph::position, UNKNOWN_TILE);
	} else if ((pos = messages.find(LEVEL_ALTAR_HERE)) != string::npos) {
		setDungeonSymbol(Saiph::position, ALTAR);
		/* set symbol value too */
		if (messages.find(" (unaligned) ", pos) != string::npos)
			symbols[(unsigned char) ALTAR][Saiph::position] = NEUTRAL;
		else if (messages.find(" (chaotic) ", pos) != string::npos)
			symbols[(unsigned char) ALTAR][Saiph::position] = CHAOTIC;
		else
			symbols[(unsigned char) ALTAR][Saiph::position] = LAWFUL;
	} else if (messages.find(LEVEL_UNDIGGABLE) != string::npos) {
		undiggable = true;
	}

	/* figure out if there's something on the ground */
	if ((pos = messages.find(LEVEL_YOU_SEE_HERE)) != string::npos) {
		/* we see a single item on ground */
		clearStash(Saiph::position);
		pos += sizeof (LEVEL_YOU_SEE_HERE) - 1;
		string::size_type length = messages.find(".  ", pos);
		if (length != string::npos) {
			length = length - pos;
			Item item(messages.substr(pos, length));
			addItemToStash(Saiph::position, item);
		}
	} else if ((pos = messages.find(LEVEL_YOU_FEEL_HERE)) != string::npos) {
		/* we feel a single item on the ground */
		clearStash(Saiph::position);
		pos += sizeof (LEVEL_YOU_FEEL_HERE) - 1;
		string::size_type length = messages.find(".  ", pos);
		if (length != string::npos) {
			length = length - pos;
			Item item(messages.substr(pos, length));
			addItemToStash(Saiph::position, item);
		}
	} else if ((pos = messages.find(LEVEL_THINGS_THAT_ARE_HERE)) != string::npos || (pos = messages.find(LEVEL_THINGS_THAT_YOU_FEEL_HERE)) != string::npos) {
		/* we see/feel multiple items on the ground */
		clearStash(Saiph::position);
		pos = messages.find("  ", pos + 1);
		while (pos != string::npos && messages.size() > pos + 2) {
			pos += 2;
			string::size_type length = messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			Item item(messages.substr(pos, length));
			addItemToStash(Saiph::position, item);
			pos += length;
		}
	} else if (messages.find(LEVEL_YOU_SEE_NO_OBJECTS) != string::npos || messages.find(LEVEL_YOU_FEEL_NO_OBJECTS) != string::npos || messages.find(LEVEL_THERE_IS_NOTHING_HERE) != string::npos) {
		/* we see/feel no items on the ground */
		stashes.erase(Saiph::position);
	} else if (!World::menu) {
		/* check if we received items */
		string::size_type pos = 0;
		string::size_type pos2 = -1;
		/* reset received list */
		received.items.clear();
		while ((pos = messages.find(" - ", pos2 + 4)) != string::npos && pos > 1 && messages[pos - 2] == ' ' && (pos2 = messages.find_first_of('.', pos + 3)) != string::npos && pos2 < messages.size() - 2 && messages[pos2 + 1] == ' ' && messages[pos2 + 2] == ' ') {
			/* add item to inventory */
			Item item(messages.substr(pos + 3, pos2 - pos - 3));
			if (item.count <= 0) {
				Debug::notice() << INVENTORY_DEBUG_NAME << "Failed parsing \"" << messages.substr(pos - 2, pos2 - pos + 2) << "\" as an item" << endl;
				continue;
			}
			Inventory::addItem(messages[pos - 1], item);
			/* add item to changed.keys */
			received.items[messages[pos - 1]] = item;
		}
		if (received.items.size() > 0) {
			/* if we're standing on a stash, mark it as changed */
			map<Point, Stash>::iterator s = World::levels[Saiph::position.level].stashes.find(Saiph::position);
			if (s != World::levels[Saiph::position.level].stashes.end())
				s->second.turn_changed = World::turn;
			/* broadcast "ChangedInventoryItems" */
			EventBus::broadcast(static_cast<Event *>(&received));
		}
	}

	/* send event if we're standing on stash */
	map<Point, Stash>::iterator s = stashes.find(Saiph::position);
	if (s != stashes.end()) {
		on_ground.items = s->second.items;
		/* broadcast "ItemsOnGround" */
		EventBus::broadcast(static_cast<Event *>(&on_ground));
	}
}

void Level::updateMapPoint(const Point &point, unsigned char symbol, int color) {
	if (branch == BRANCH_ROGUE) {
		/* we need a special symbol remapping for rogue level */
		switch ((char) symbol) {
			case '+':
				symbol = OPEN_DOOR;
				break;

			case ':':
				symbol = FOOD;
				break;

			case ']':
				symbol = ARMOR;
				break;

			case '*':
				symbol = GOLD;
				break;

			case ',':
				symbol = AMULET;
				break;

			case '%':
				/* set symbol to ROGUE_STAIRS if we don't know where the stairs lead.
				 * if we do know where the stairs lead, then set the symbol accordingly */
				if (dungeonmap[point.row][point.col] != STAIRS_DOWN && dungeonmap[point.row][point.col] != STAIRS_UP)
					symbol = ROGUE_STAIRS;
				else
					symbol = dungeonmap[point.row][point.col];
				break;

			default:
				break;
		}
	} else {
		/* remap ambigous symbols */
		symbol = uniquemap[symbol][color];
		/* some special cases */
		if (symbol == FOUNTAIN && branch == BRANCH_MINES)
			symbol = MINES_FOUNTAIN; // to avoid dipping & such
		else if (symbol == FLOOR && dungeonmap[point.row][point.col] == SHOP_TILE)
			symbol = SHOP_TILE; // don't overwrite shop tiles here
	}
	if (dungeon[symbol] || (symbol == SOLID_ROCK && dungeonmap[point.row][point.col] == CORRIDOR)) {
		/* update the map showing static stuff */
		setDungeonSymbol(point, symbol);
	} else if (symbol != SOLID_ROCK && !passable[dungeonmap[point.row][point.col]] && dungeonmap[point.row][point.col] != UNKNOWN_TILE_UNPASSABLE) {
		/* we can't see the floor here, but we believe we can pass this tile.
		 * place an UNKNOWN_TILE here.
		 * the reason we check if stored tile is !passable is because if we don't,
		 * then every tile a monster steps on or drops an item on will become UNKNOWN_TILE,
		 * even if we already know what's beneath the monster/item. */
		setDungeonSymbol(point, UNKNOWN_TILE);
	}
	/* update items */
	if (!Saiph::hallucinating && item[symbol]) {
		map<Point, Stash>::iterator s = stashes.find(point);
		if (s != stashes.end()) {
			if ((s->second.top_symbol != symbol || s->second.top_color != color)) {
				/* top symbol/color changed, update */
				if (s->second.top_symbol != ILLEGAL_ITEM)
					s->second.turn_changed = World::turn;
				s->second.top_symbol = symbol;
				s->second.top_color = color;
			}
		} else {
			/* new stash */
			stashes[point] = Stash(World::turn, symbol, color);
		}
	} else if (symbol == dungeonmap[point.row][point.col]) {
		/* if there ever was a stash here, it's gone now */
		stashes.erase(point);
	}

	/* update monsters */
	if (monster[symbol] && point != Saiph::position) {
		/* add a monster, or update position of an existing monster */
		unsigned char msymbol;
		if ((color >= INVERSE_BLACK && color <= INVERSE_WHITE) || (color >= INVERSE_BOLD_BLACK && color <= INVERSE_BOLD_WHITE))
			msymbol = PET;
		else
			msymbol = symbol;
		/* find nearest monster */
		int min_distance = INT_MAX;
		map<Point, Monster>::iterator nearest = monsters.end();
		for (map<Point, Monster>::iterator m = monsters.begin(); m != monsters.end(); ++m) {
			if (m->second.symbol != msymbol || m->second.color != color)
				continue; // not the same monster
			unsigned char old_symbol;
			if ((color >= INVERSE_BLACK && color <= INVERSE_WHITE) || (color >= INVERSE_BOLD_BLACK && color <= INVERSE_BOLD_WHITE))
				old_symbol = PET;
			else
				old_symbol = World::view[m->first.row][m->first.col];
			if (m->second.symbol == old_symbol && m->second.color == World::color[m->first.row][m->first.col]) {
				/* note about this "point == m->first":
				 * the character for the monster may be updated even if it hasn't moved,
				 * if this is the case, we should return and neither move nor add the
				 * monster as that will screw up the data we know about the monster */
				if (point == m->first)
					return;
				continue; // this monster already is on its square
			}
			/* see if this monster is closer than the last found monster */
			int distance = max(abs(m->first.row - point.row), abs(m->first.col - point.col));
			if (distance > MAX_MONSTER_MOVE)
				continue; // too far away from where we last saw it, probably new monster
			else if (distance >= min_distance)
				continue;
			else if ((m->second.priest || m->second.shopkeeper) && distance > 1)
				continue; // shopkeepers and priests are very close to eachother in minetown
			/* it is */
			min_distance = distance;
			nearest = m;
		}
		if (nearest != monsters.end()) {
			/* we know of this monster, move it to new location */
			/* remove monster from monstermap */
			monstermap[nearest->first.row][nearest->first.col] = ILLEGAL_MONSTER;
			/* update monster */
			nearest->second.last_seen = World::turn;
			monsters[point] = nearest->second;
			monsters.erase(nearest);
		} else {
			/* add monster */
			monsters[point] = Monster(msymbol, color, World::turn);
		}
		/* set monster on monstermap */
		monstermap[point.row][point.col] = msymbol;
	}
}

void Level::updateMonsters() {
	/* remove monsters that seems to be gone
	 * and make monsters we can't see !visible */
	for (map<Point, Monster>::iterator m = monsters.begin(); m != monsters.end(); ) {
		unsigned char symbol;
		int color = World::color[m->first.row][m->first.col];
		if ((color >= INVERSE_BLACK && color <= INVERSE_WHITE) || (color >= INVERSE_BOLD_BLACK && color <= INVERSE_BOLD_WHITE))
			symbol = PET;
		else
			symbol = World::view[m->first.row][m->first.col];
		/* if we don't see the monster on world->view then it's not visible */
		m->second.visible = (m->first != Saiph::position && symbol == m->second.symbol && color == m->second.color);
		if (m->second.visible) {
			/* monster still visible, don't remove it */
			++m;
			continue;
		} else if (abs(Saiph::position.row - m->first.row) > 1 || abs(Saiph::position.col - m->first.col) > 1) {
			/* player is not next to where we last saw the monster */
			++m;
			continue;
		}
		/* remove monster from monstermap */
		monstermap[m->first.row][m->first.col] = ILLEGAL_MONSTER;
		/* remove monster from list */
		monsters.erase(m++);
	}
}

void Level::updatePathMap() {
	/* reset all nodes */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c)
			pathmap[r][c] = PathNode();
	}

	/* create pathmap */
	int curnode = 0;
	int nodes = 0;
	unsigned int cost = 0;
	Point from = Saiph::position;
	pathmap[from.row][from.col].dir = NOWHERE;
	pathmap[from.row][from.col].moves = 0;
	pathmap[from.row][from.col].cost = 0;

	/* first move northwest node */
	Point to(from.row - 1, from.col - 1);
	if (to.row >= MAP_ROW_BEGIN && to.col >= MAP_COL_BEGIN) {
		cost = updatePathMapHelper(to, from);
		if (cost < UNREACHABLE) {
			pathing_queue[nodes++] = to;
			pathmap[to.row][to.col] = PathNode(from, NW, 1, cost);
		} else {
			pathmap[to.row][to.col] = PathNode(from, NW, 1, UNPASSABLE);
		}
	}
	/* first move north node */
	++to.col;
	if (to.row >= MAP_ROW_BEGIN) {
		cost = updatePathMapHelper(to, from);
		if (cost < UNREACHABLE) {
			pathing_queue[nodes++] = to;
			pathmap[to.row][to.col] = PathNode(from, N, 1, cost);
		} else {
			pathmap[to.row][to.col] = PathNode(from, N, 1, UNPASSABLE);
		}
	}
	/* first move northeast node */
	++to.col;
	if (to.row >= MAP_ROW_BEGIN && to.col <= MAP_COL_END) {
		cost = updatePathMapHelper(to, from);
		if (cost < UNREACHABLE) {
			pathing_queue[nodes++] = to;
			pathmap[to.row][to.col] = PathNode(from, NE, 1, cost);
		} else {
			pathmap[to.row][to.col] = PathNode(from, NE, 1, UNPASSABLE);
		}
	}
	/* first move east node */
	++to.row;
	if (to.col <= MAP_COL_END) {
		cost = updatePathMapHelper(to, from);
		if (cost < UNREACHABLE) {
			pathing_queue[nodes++] = to;
			pathmap[to.row][to.col] = PathNode(from, E, 1, cost);
		} else {
			pathmap[to.row][to.col] = PathNode(from, E, 1, UNPASSABLE);
		}
	}
	/* first move southeast node */
	++to.row;
	if (to.row <= MAP_ROW_END && to.col <= MAP_COL_END) {
		cost = updatePathMapHelper(to, from);
		if (cost < UNREACHABLE) {
			pathing_queue[nodes++] = to;
			pathmap[to.row][to.col] = PathNode(from, SE, 1, cost);
		} else {
			pathmap[to.row][to.col] = PathNode(from, SE, 1, UNPASSABLE);
		}
	}
	/* first move south node */
	--to.col;
	if (to.row <= MAP_ROW_END) {
		cost = updatePathMapHelper(to, from);
		if (cost < UNREACHABLE) {
			pathing_queue[nodes++] = to;
			pathmap[to.row][to.col] = PathNode(from, S, 1, cost);
		} else {
			pathmap[to.row][to.col] = PathNode(from, S, 1, UNPASSABLE);
		}
	}
	/* first move southwest node */
	--to.col;
	if (to.row <= MAP_ROW_END && to.col >= MAP_COL_BEGIN) {
		cost = updatePathMapHelper(to, from);
		if (cost < UNREACHABLE) {
			pathing_queue[nodes++] = to;
			pathmap[to.row][to.col] = PathNode(from, SW, 1, cost);
		} else {
			pathmap[to.row][to.col] = PathNode(from, SW, 1, UNPASSABLE);
		}
	}
	/* first move west node */
	--to.row;
	if (to.col >= MAP_COL_BEGIN) {
		cost = updatePathMapHelper(to, from);
		if (cost < UNREACHABLE) {
			pathing_queue[nodes++] = to;
			pathmap[to.row][to.col] = PathNode(from, W, 1, cost);
		} else {
			pathmap[to.row][to.col] = PathNode(from, W, 1, UNPASSABLE);
		}
	}

	/* calculate remaining nodes */
	while (curnode < nodes) {
		from = pathing_queue[curnode++];
		/* check northwest node */
		to = Point(from.row - 1, from.col - 1);
		if (to.row >= MAP_ROW_BEGIN && to.col >= MAP_COL_BEGIN) {
			cost = updatePathMapHelper(to, from);
			if (cost < pathmap[to.row][to.col].cost) {
				pathing_queue[nodes++] = to;
				pathmap[to.row][to.col] = PathNode(from, pathmap[from.row][from.col].dir, pathmap[from.row][from.col].moves + 1, cost);
			} else if (cost == pathmap[to.row][to.col].cost && cost == UNREACHABLE) {
				pathmap[to.row][to.col] = PathNode(from, pathmap[from.row][from.col].dir, pathmap[from.row][from.col].moves + 1, UNPASSABLE);
			}
		}
		/* check north node */
		++to.col;
		if (to.row >= MAP_ROW_BEGIN) {
			cost = updatePathMapHelper(to, from);
			if (cost < pathmap[to.row][to.col].cost) {
				pathing_queue[nodes++] = to;
				pathmap[to.row][to.col] = PathNode(from, pathmap[from.row][from.col].dir, pathmap[from.row][from.col].moves + 1, cost);
			} else if (cost == pathmap[to.row][to.col].cost && cost == UNREACHABLE) {
				pathmap[to.row][to.col] = PathNode(from, pathmap[from.row][from.col].dir, pathmap[from.row][from.col].moves + 1, UNPASSABLE);
			}
		}
		/* check northeast node */
		++to.col;
		if (to.row >= MAP_ROW_BEGIN && to.col <= MAP_COL_END) {
			cost = updatePathMapHelper(to, from);
			if (cost < pathmap[to.row][to.col].cost) {
				pathing_queue[nodes++] = to;
				pathmap[to.row][to.col] = PathNode(from, pathmap[from.row][from.col].dir, pathmap[from.row][from.col].moves + 1, cost);
			} else if (cost == pathmap[to.row][to.col].cost && cost == UNREACHABLE) {
				pathmap[to.row][to.col] = PathNode(from, pathmap[from.row][from.col].dir, pathmap[from.row][from.col].moves + 1, UNPASSABLE);
			}
		}
		/* check east node */
		++to.row;
		if (to.col <= MAP_COL_END) {
			cost = updatePathMapHelper(to, from);
			if (cost < pathmap[to.row][to.col].cost) {
				pathing_queue[nodes++] = to;
				pathmap[to.row][to.col] = PathNode(from, pathmap[from.row][from.col].dir, pathmap[from.row][from.col].moves + 1, cost);
			} else if (cost == pathmap[to.row][to.col].cost && cost == UNREACHABLE) {
				pathmap[to.row][to.col] = PathNode(from, pathmap[from.row][from.col].dir, pathmap[from.row][from.col].moves + 1, UNPASSABLE);
			}
		}
		/* check southeast node */
		++to.row;
		if (to.row <= MAP_ROW_END && to.col <= MAP_COL_END) {
			cost = updatePathMapHelper(to, from);
			if (cost < pathmap[to.row][to.col].cost) {
				pathing_queue[nodes++] = to;
				pathmap[to.row][to.col] = PathNode(from, pathmap[from.row][from.col].dir, pathmap[from.row][from.col].moves + 1, cost);
			} else if (cost == pathmap[to.row][to.col].cost && cost == UNREACHABLE) {
				pathmap[to.row][to.col] = PathNode(from, pathmap[from.row][from.col].dir, pathmap[from.row][from.col].moves + 1, UNPASSABLE);
			}
		}
		/* check south node */
		--to.col;
		if (to.row <= MAP_ROW_END) {
			cost = updatePathMapHelper(to, from);
			if (cost < pathmap[to.row][to.col].cost) {
				pathing_queue[nodes++] = to;
				pathmap[to.row][to.col] = PathNode(from, pathmap[from.row][from.col].dir, pathmap[from.row][from.col].moves + 1, cost);
			} else if (cost == pathmap[to.row][to.col].cost && cost == UNREACHABLE) {
				pathmap[to.row][to.col] = PathNode(from, pathmap[from.row][from.col].dir, pathmap[from.row][from.col].moves + 1, UNPASSABLE);
			}
		}
		/* check southwest node */
		--to.col;
		if (to.row <= MAP_ROW_END && to.col >= MAP_COL_BEGIN) {
			cost = updatePathMapHelper(to, from);
			if (cost < pathmap[to.row][to.col].cost) {
				pathing_queue[nodes++] = to;
				pathmap[to.row][to.col] = PathNode(from, pathmap[from.row][from.col].dir, pathmap[from.row][from.col].moves + 1, cost);
			} else if (cost == pathmap[to.row][to.col].cost && cost == UNREACHABLE) {
				pathmap[to.row][to.col] = PathNode(from, pathmap[from.row][from.col].dir, pathmap[from.row][from.col].moves + 1, UNPASSABLE);
			}
		}
		/* check west node */
		--to.row;
		if (to.col >= MAP_COL_BEGIN) {
			cost = updatePathMapHelper(to, from);
			if (cost < pathmap[to.row][to.col].cost) {
				pathing_queue[nodes++] = to;
				pathmap[to.row][to.col] = PathNode(from, pathmap[from.row][from.col].dir, pathmap[from.row][from.col].moves + 1, cost);
			} else if (cost == pathmap[to.row][to.col].cost && cost == UNREACHABLE) {
				pathmap[to.row][to.col] = PathNode(from, pathmap[from.row][from.col].dir, pathmap[from.row][from.col].moves + 1, UNPASSABLE);
			}
		}
	}
}

/* private methods */
void Level::addItemToStash(const Point &point, const Item &item) {
	Debug::notice() << LEVEL_DEBUG_NAME << "Adding " << item.count << " " << item.name << " to stash at " << point.row << ", " << point.col << endl;
	if (item.count <= 0)
		return;
	map<Point, Stash>::iterator s = stashes.find(point);
	if (s != stashes.end()) {
		s->second.items.push_back(item);
		return;
	}
	/* new stash */
	Stash stash(World::turn);
	stash.items.push_back(item);
	stashes[point] = stash;
}

void Level::clearStash(const Point &point) {
	/* clear the contents of a stash */
	map<Point, Stash>::iterator s = stashes.find(point);
	if (s != stashes.end()) {
		s->second.items.clear();
		s->second.top_symbol = ILLEGAL_ITEM;
	}
}

unsigned int Level::updatePathMapHelper(const Point &to, const Point &from) {
	/* helper method for updatePathMap()
	 * return UNREACHABLE if move is illegal, or the cost for reaching the node if move is legal */
	unsigned char s = dungeonmap[to.row][to.col];
	if (!passable[s])
		return UNREACHABLE;
	bool cardinal_move = (to.row == from.row || to.col == from.col);
	if (!cardinal_move) {
		if (s == OPEN_DOOR || dungeonmap[from.row][from.col] == OPEN_DOOR)
			return UNREACHABLE; // diagonally in/out of door
		if (s == UNKNOWN_TILE_DIAGONALLY_UNPASSABLE || dungeonmap[from.row][from.col] == UNKNOWN_TILE_DIAGONALLY_UNPASSABLE)
			return UNREACHABLE; // don't know what tile this is, but we know we can't pass it diagonally
		unsigned char sc1 = dungeonmap[to.row][from.col];
		unsigned char sc2 = dungeonmap[from.row][to.col];
		if (!passable[sc1] && !passable[sc2]) {
			/* moving past two corners
			 * while we may pass two corners if we're not carrying too much we'll just ignore this.
			 * it's bound to cause issues */
			if (sc1 != BOULDER && sc2 != BOULDER)
				return UNREACHABLE; // neither corner is a boulder, we may not pass
			else if (branch == BRANCH_SOKOBAN)
				return UNREACHABLE; // in sokoban we can't pass by boulders diagonally
		}
		//if (polymorphed_to_grid_bug)
		//      return UNREACHABLE;
	}
	if (s == LAVA) // && !levitating)
	      return UNREACHABLE;
	if (s == WATER) // && (!levitating || !waterwalk))
	      return UNREACHABLE;
	if (s == TRAP && branch == BRANCH_SOKOBAN)
		return UNREACHABLE;
	if (monstermap[to.row][to.col] != ILLEGAL_MONSTER && abs(Saiph::position.row - to.row) <= 1 && abs(Saiph::position.col - to.col) <= 1)
		return UNREACHABLE; // don't path through monster next to her
	unsigned int cost = pathmap[from.row][from.col].cost + (cardinal_move ? COST_CARDINAL : COST_DIAGONAL);
	cost += pathcost[s];
	if (monstermap[to.row][to.col] != ILLEGAL_MONSTER)
		cost += COST_MONSTER;
	return cost;
}

/* private static methods */
void Level::init() {
	initialized = true;
	/* monsters */
	for (int a = 0; a <= UCHAR_MAX; ++a) {
		if ((a >= '@' && a <= 'Z') || (a >= 'a' && a <= 'z') || (a >= '1' && a <= '5')  || a == '&' || a == '\'' || a == ':' || a == ';' || a == '~' || a == PET)
			monster[a] = true;
	}
	/* items */
	item[(unsigned char) WEAPON] = true;
	item[(unsigned char) ARMOR] = true;
	item[(unsigned char) RING] = true;
	item[(unsigned char) AMULET] = true;
	item[(unsigned char) TOOL] = true;
	item[(unsigned char) FOOD] = true;
	item[(unsigned char) POTION] = true;
	item[(unsigned char) SCROLL] = true;
	item[(unsigned char) SPELLBOOK] = true;
	item[(unsigned char) WAND] = true;
	item[(unsigned char) GOLD] = true;
	item[(unsigned char) GEM] = true;
	item[(unsigned char) STATUE] = true;
	// skipping boulder as that's a special item
	item[(unsigned char) IRON_BALL] = true;
	item[(unsigned char) CHAINS] = true;
	item[(unsigned char) VENOM] = true;
	/* stuff we can walk on */
	passable[(unsigned char) FLOOR] = true;
	passable[(unsigned char) OPEN_DOOR] = true;
	passable[(unsigned char) CORRIDOR] = true;
	passable[(unsigned char) STAIRS_UP] = true;
	passable[(unsigned char) STAIRS_DOWN] = true;
	passable[(unsigned char) ALTAR] = true;
	passable[(unsigned char) GRAVE] = true;
	passable[(unsigned char) THRONE] = true;
	passable[(unsigned char) SINK] = true;
	passable[(unsigned char) FOUNTAIN] = true;
	passable[(unsigned char) WATER] = true;
	passable[(unsigned char) ICE] = true;
	passable[(unsigned char) LAVA] = true;
	passable[(unsigned char) LOWERED_DRAWBRIDGE] = true;
	passable[(unsigned char) TRAP] = true;
	passable[(unsigned char) UNKNOWN_TILE] = true;
	passable[(unsigned char) UNKNOWN_TILE_DIAGONALLY_UNPASSABLE] = true;
	passable[(unsigned char) ROGUE_STAIRS] = true;
	passable[(unsigned char) MINES_FOUNTAIN] = true;
	passable[(unsigned char) SHOP_TILE] = true;
	passable[(unsigned char) WEAPON] = true;
	passable[(unsigned char) ARMOR] = true;
	passable[(unsigned char) RING] = true;
	passable[(unsigned char) AMULET] = true;
	passable[(unsigned char) TOOL] = true;
	passable[(unsigned char) FOOD] = true;
	passable[(unsigned char) POTION] = true;
	passable[(unsigned char) SCROLL] = true;
	passable[(unsigned char) SPELLBOOK] = true;
	passable[(unsigned char) WAND] = true;
	passable[(unsigned char) GOLD] = true;
	passable[(unsigned char) GEM] = true;
	passable[(unsigned char) STATUE] = true;
	passable[(unsigned char) IRON_BALL] = true;
	passable[(unsigned char) CHAINS] = true;
	passable[(unsigned char) VENOM] = true;
	/* dungeon layout */
	dungeon[(unsigned char) VERTICAL_WALL] = true;
	dungeon[(unsigned char) HORIZONTAL_WALL] = true;
	dungeon[(unsigned char) FLOOR] = true;
	dungeon[(unsigned char) OPEN_DOOR] = true;
	dungeon[(unsigned char) CLOSED_DOOR] = true;
	dungeon[(unsigned char) IRON_BARS] = true;
	dungeon[(unsigned char) TREE] = true;
	dungeon[(unsigned char) CORRIDOR] = true;
	dungeon[(unsigned char) STAIRS_UP] = true;
	dungeon[(unsigned char) STAIRS_DOWN] = true;
	dungeon[(unsigned char) ALTAR] = true;
	dungeon[(unsigned char) GRAVE] = true;
	dungeon[(unsigned char) THRONE] = true;
	dungeon[(unsigned char) SINK] = true;
	dungeon[(unsigned char) FOUNTAIN] = true;
	dungeon[(unsigned char) WATER] = true;
	dungeon[(unsigned char) ICE] = true;
	dungeon[(unsigned char) LAVA] = true;
	dungeon[(unsigned char) LOWERED_DRAWBRIDGE] = true;
	dungeon[(unsigned char) RAISED_DRAWBRIDGE] = true;
	dungeon[(unsigned char) TRAP] = true;
	dungeon[(unsigned char) BOULDER] = true; // hardly static, but we won't allow moving on to one
	dungeon[(unsigned char) ROGUE_STAIRS] = true; // unique, is both up & down stairs
	dungeon[(unsigned char) MINES_FOUNTAIN] = true; // unique, but [mostly] static
	dungeon[(unsigned char) SHOP_TILE] = true; // unique, but [mostly] static
	/* cost for pathing on certain tiles */
	pathcost[(unsigned char) FOUNTAIN] = COST_FOUNTAIN;
	pathcost[(unsigned char) GRAVE] = COST_GRAVE;
	pathcost[(unsigned char) ALTAR] = COST_ALTAR;
	pathcost[(unsigned char) ICE] = COST_ICE;
	pathcost[(unsigned char) LAVA] = COST_LAVA;
	pathcost[(unsigned char) MINES_FOUNTAIN] = COST_FOUNTAIN;
	pathcost[(unsigned char) TRAP] = COST_TRAP;
	pathcost[(unsigned char) WATER] = COST_WATER;
	/* remapping ambigous symbols */
	for (int s = 0; s <= UCHAR_MAX; ++s) {
		for (int c = 0; c <= CHAR_MAX; ++c)
			uniquemap[s][c] = s;
	}
	uniquemap[(unsigned char) CORRIDOR][CYAN] = IRON_BARS;
	uniquemap[(unsigned char) CORRIDOR][GREEN] = TREE;
	uniquemap[(unsigned char) FLOOR][CYAN] = ICE;
	uniquemap[(unsigned char) FLOOR][YELLOW] = LOWERED_DRAWBRIDGE;
	uniquemap[(unsigned char) FOUNTAIN][NO_COLOR] = SINK;
	uniquemap[(unsigned char) GRAVE][YELLOW] = THRONE;
	uniquemap[(unsigned char) HORIZONTAL_WALL][YELLOW] = OPEN_DOOR;
	uniquemap[(unsigned char) VERTICAL_WALL][YELLOW] = OPEN_DOOR;
	uniquemap[(unsigned char) WATER][RED] = LAVA;
	uniquemap[(unsigned char) TRAP][BOLD_MAGENTA] = MAGIC_PORTAL;
}
