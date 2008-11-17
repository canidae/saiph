#include <stdlib.h>
#include "Debug.h"
#include "Item.h"
#include "Level.h"
#include "Saiph.h"
#include "World.h"

using namespace std;

/* initialize static variables */
/* public */
bool Level::passable[UCHAR_MAX + 1] = {false};
bool Level::track_symbol[UCHAR_MAX + 1] = {false};
/* private */
Point Level::pathing_queue[PATHING_QUEUE_SIZE] = {Point()};
unsigned char Level::uniquemap[UCHAR_MAX + 1][CHAR_MAX + 1] = {{0}};
int Level::pathcost[UCHAR_MAX + 1] = {0};
bool Level::dungeon[UCHAR_MAX + 1] = {false};
bool Level::monster[UCHAR_MAX + 1] = {false};
bool Level::item[UCHAR_MAX + 1] = {false};
bool Level::initialized = false;

/* constructors/destructor */
Level::Level(Saiph *saiph, string name, int branch) : name(name), branch(branch), saiph(saiph) {
	for (int a = 0; a < MAP_ROW_END + 1; ++a) {
		for (int b = 0; b < MAP_COL_END + 1; ++b) {
			dungeonmap[a][b] = SOLID_ROCK;
			monstermap[a][b] = ILLEGAL_MONSTER;
		}
	}
	sscanf(name.c_str(), "%*[^0123456789]%d", &depth);
	if (!initialized)
		init();
}

/* methods */
void Level::parseMessages(const string &messages) {
	/* set inventory_changed to false */
	saiph->inventory_changed = false;
	/* set got_[drop|pickup]_menu to false if we don't have a menu */
	if (!saiph->world->menu) {
		saiph->got_drop_menu = false;
		saiph->got_pickup_menu = false;
	}
	/* if last command was ":" we should clear stash on ground */
	if (saiph->last_command == ":")
		clearStash(saiph->position);
	/* parse messages that can help us find doors/staircases/etc. */
	string::size_type pos;
	if (messages.find(LEVEL_STAIRCASE_UP_HERE, 0) != string::npos)
		setDungeonSymbol(saiph->position, STAIRS_UP);
	else if (messages.find(LEVEL_STAIRCASE_DOWN_HERE, 0) != string::npos)
		setDungeonSymbol(saiph->position, STAIRS_DOWN);
	else if (messages.find(LEVEL_OPEN_DOOR_HERE, 0) != string::npos)
		setDungeonSymbol(saiph->position, OPEN_DOOR);
	else if (messages.find(LEVEL_GRAVE_HERE, 0) != string::npos)
		setDungeonSymbol(saiph->position, GRAVE);
	else if (messages.find(LEVEL_FOUNTAIN_HERE, 0) != string::npos)
		setDungeonSymbol(saiph->position, FOUNTAIN);
	else if (messages.find(LEVEL_FOUNTAIN_DRIES_UP, 0) != string::npos || messages.find(LEVEL_FOUNTAIN_DRIES_UP2, 0) != string::npos)
		setDungeonSymbol(saiph->position, FLOOR);
	else if (messages.find(LEVEL_NO_STAIRS_DOWN_HERE, 0) != string::npos || messages.find(LEVEL_NO_STAIRS_UP_HERE, 0) != string::npos)
		setDungeonSymbol(saiph->position, UNKNOWN_TILE);
	else if ((pos = messages.find(LEVEL_ALTAR_HERE, 0)) != string::npos) {
		if (messages.find(" (unaligned) ", pos) != string::npos)
			setDungeonSymbol(saiph->position, ALTAR, NEUTRAL);
		else if (messages.find(" (chaotic) ", pos) != string::npos)
			setDungeonSymbol(saiph->position, ALTAR, CHAOTIC);
		else
			setDungeonSymbol(saiph->position, ALTAR, LAWFUL);
	} else if (dungeonmap[saiph->position.row][saiph->position.col] == UNKNOWN_TILE) {
		/* when we've checked messages for static dungeon features and not found anything,
		 * then we can set the tile to UNKNOWN_TILE_DIAGONALLY_PASSABLE if the tile is UNKNOWN_TILE */
		setDungeonSymbol(saiph->position, UNKNOWN_TILE_DIAGONALLY_PASSABLE);
	}

	/* item parsing */
	/* figure out if there's something on the ground or if we're picking up something */
	if ((pos = messages.find(LEVEL_YOU_SEE_HERE, 0)) != string::npos || (pos = messages.find(LEVEL_YOU_FEEL_HERE, 0)) != string::npos) {
		/* single item on ground */
		clearStash(saiph->position);
		if (messages.find(LEVEL_YOU_SEE_HERE, 0) != string::npos)
			pos += sizeof (LEVEL_YOU_SEE_HERE) - 1;
		else
			pos += sizeof (LEVEL_YOU_FEEL_HERE) - 1;
		string::size_type length = messages.find(".  ", pos);
		if (length != string::npos) {
			length = length - pos;
			Item item(messages.substr(pos, length));
			addItemToStash(saiph->position, item);
		}
	} else if ((pos = messages.find(LEVEL_THINGS_THAT_ARE_HERE, 0)) != string::npos || (pos = messages.find(LEVEL_THINGS_THAT_YOU_FEEL_HERE, 0)) != string::npos) {
		/* multiple items on ground */
		clearStash(saiph->position);
		pos = messages.find("  ", pos + 1);
		while (pos != string::npos && messages.size() > pos + 2) {
			pos += 2;
			string::size_type length = messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			Item item(messages.substr(pos, length));
			addItemToStash(saiph->position, item);
			pos += length;
		}
	} else if (messages.find(LEVEL_YOU_SEE_NO_OBJECTS, 0) != string::npos || messages.find(LEVEL_YOU_FEEL_NO_OBJECTS, 0) != string::npos || messages.find(LEVEL_THERE_IS_NOTHING_HERE, 0) != string::npos) {
		/* no items on ground */
		stashes.erase(saiph->position);
	} else if ((pos = messages.find(MESSAGE_PICK_UP_WHAT, 0)) != string::npos || saiph->got_pickup_menu) {
		/* picking up stuff */
		if (saiph->got_pickup_menu) {
			/* not the first page, set pos to 0 */
			pos = 0;
		} else {
			/* first page */
			saiph->got_pickup_menu = true;
			/* and find first "  " */
			pos = messages.find("  ", pos + 1);
		}
		while (pos != string::npos && messages.size() > pos + 6) {
			pos += 6;
			string::size_type length = messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			if (messages[pos - 2] == '-') {
				Item item(messages.substr(pos, length));
				saiph->pickup[messages[pos - 4]] = item;
			}
			pos += length;
		}
	} else if ((pos = messages.find(MESSAGE_DROP_WHICH_ITEMS, 0)) != string::npos || saiph->got_drop_menu) {
		/* dropping items */
		if (saiph->got_drop_menu) {
			/* not the first page, set pos to 0 */
			pos = 0;
		} else {
			/* first page, set menu */
			saiph->got_drop_menu = true;;
			/* and find first "  " */
			pos = messages.find("  ", pos + 1);
		}
		while (pos != string::npos && messages.size() > pos + 6) {
			pos += 6;
			string::size_type length = messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			if (messages[pos - 2] == '-')
				saiph->drop[messages[pos - 4]] = Item(messages.substr(pos, length));
			pos += length;
		}
	} else if (messages.find(MESSAGE_NOT_CARRYING_ANYTHING, 0) != string::npos || messages.find(MESSAGE_NOT_CARRYING_ANYTHING_EXCEPT_GOLD, 0) != string::npos) {
		/* our inventory is empty. how did that happen? */
		saiph->inventory.clear();
		saiph->inventory_changed = true;
	} else if ((pos = messages.find(".  ", 0)) != string::npos) {
		/* when we pick up stuff we only get "  f - a lichen corpse.  " and similar.
		 * we'll need to handle this too somehow.
		 * when we're burdened we'll get "  You have a little trouble lifting f - a lichen corpse.  ".
		 * we're searching for ".  " as we won't get that when we're listing inventory.
		 * also, this won't detect gold, but we might not need to detect that,
		 * well, it's gonna be a bit buggy when picking up gold from stashes */
		/* additionally, we'll assume we're picking up from the stash at this location.
		 * this will also trigger on wishes, but meh, probably not gonna be an issue */
		pos = 0;
		int pickup_count = 0;
		while ((pos = messages.find(" - ", pos)) != string::npos) {
			if (pos > 2 && (messages[pos - 3] == ' ' || messages[pos - 3] == 'g') && messages[pos - 2] == ' ') {
				unsigned char key = messages[pos - 1];
				pos += 3;
				string::size_type length = messages.find(".  ", pos);
				if (length == string::npos)
					break;
				length = length - pos;
				Item item(messages.substr(pos, length));
				if (saiph->addItemToInventory(key, item))
					++pickup_count;
				pos += length;
			} else {
				/* "Yak - dog food!" mess things up.
				 * this is why we checked for "  " or "g " before the "-".
				 * we may get "... trouble lifting f - ya", and we need to detect that,
				 * hence the "g " */
				++pos;
			}
		}
		if (pickup_count > 0)
			saiph->inventory_changed = true;
		if ((int) stashes[saiph->position].items.size() == pickup_count) {
			/* we probably picked up everything here, remove stash */
			stashes.erase(saiph->position);
		}
	} else if ((pos = messages.find(" - ", 0)) != string::npos && messages.find(" -  ", 0) == string::npos) {
		/* we probably listed our inventory */
		/* we're searching for " -  " because when we #enhance there are 2 spaces after the "-".
		 * otherwise we'll confuse the inventory list with the enhance list, which is very bad */
		if (saiph->world->cur_page == 1)
			saiph->inventory.clear(); // only clear when we're listing 1st page
		while ((pos = messages.find(" - ", pos)) != string::npos) {
			if (pos > 2 && messages[pos - 3] == ' ' && messages[pos - 2] == ' ') {
				unsigned char key = messages[pos - 1];
				pos += 3;
				string::size_type length = messages.find("  ", pos);
				if (length == string::npos)
					break;
				length = length - pos;
				Item item(messages.substr(pos, length));
				saiph->addItemToInventory(key, item);
				pos += length;
			}
		}
		saiph->inventory_changed = true;
	}
}

void Level::setDungeonSymbol(const Point &point, unsigned char symbol, int value) {
	/* since we're gonna track certain symbols we'll use an own method for this */
	if (dungeonmap[point.row][point.col] == symbol)
		return; // no change
	if (track_symbol[dungeonmap[point.row][point.col]])
		symbols[dungeonmap[point.row][point.col]].erase(point);
	if (track_symbol[symbol])
		symbols[symbol][point] = value;
	dungeonmap[point.row][point.col] = symbol;
}

void Level::setDungeonSymbolValue(const Point &point, int value) {
	/* set the value of the symbol on given point */
	if (track_symbol[dungeonmap[point.row][point.col]] && symbols[dungeonmap[point.row][point.col]].find(point) != symbols[dungeonmap[point.row][point.col]].end())
		symbols[dungeonmap[point.row][point.col]][point] = value;
}

unsigned char Level::shortestPath(const Point &target, bool allow_illegal_last_move, int *moves) {
	/* returns next move in shortest path to target.
	 * also sets "moves" to amount of moves required */
	if (target.row < MAP_ROW_BEGIN || target.row > MAP_ROW_END || target.col < MAP_COL_BEGIN || target.col > MAP_COL_END)
		return ILLEGAL_DIRECTION; // outside the map
	const PathNode *node = &pathmap[target.row][target.col];
	*moves = 0;
	if (node->cost == 0)
		return NOWHERE; // pathing to center of map?
	unsigned char move = ILLEGAL_DIRECTION;
	if (allow_illegal_last_move && node->nextrow == -1) {
		/* sometimes we wish to move somewhere we really can't move to.
		 * for example: fighting a monster in a wall or through "corner".
		 * solution: find adjacent squares with lowest cost and backtrack from there */
		/* note:
		 * since we're moving from target towards the "center" of the pathmap,
		 * we're moving the opposite direction of the node we're checking */
		int row = target.row - 1;
		int col = target.col - 1;
		unsigned int lowest_cost = UINT_MAX;
		/* northwest node */
		if (row >= MAP_ROW_BEGIN && col >= MAP_COL_BEGIN) {
			move = SE;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* north node */
		++col;
		if (row >= MAP_ROW_BEGIN && pathmap[row][col].cost < lowest_cost) {
			move = S;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* northeast node */
		++col;
		if (row >= MAP_ROW_BEGIN && col <= MAP_COL_END && pathmap[row][col].cost < lowest_cost) {
			move = SW;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* east node */
		++row;
		if (col <= MAP_COL_END && pathmap[row][col].cost < lowest_cost) {
			move = W;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* southeast node */
		++row;
		if (row <= MAP_ROW_END && col <= MAP_COL_END && pathmap[row][col].cost < lowest_cost) {
			move = NW;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* south node */
		--col;
		if (row <= MAP_ROW_END && pathmap[row][col].cost < lowest_cost) {
			move = N;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* southwest node */
		--col;
		if (row <= MAP_ROW_END && col >= MAP_COL_BEGIN && pathmap[row][col].cost < lowest_cost) {
			move = NE;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* west node */
		--row;
		if (col >= MAP_COL_BEGIN && pathmap[row][col].cost < lowest_cost) {
			move = E;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		++*moves;
		if (lowest_cost == 0)
			return move; // found the center
	}
	if (node->nextrow == -1)
		return ILLEGAL_DIRECTION; // couldn't find path

	while (node->nextrow != -1) {
		move = node->move;
		++*moves;
		node = &pathmap[node->nextrow][node->nextcol];
	}
	return move;
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
				symbol = ROGUE_STAIRS;
				break;

			default:
				break;
		}
	} else {
		/* remap ambigous symbols */
		symbol = uniquemap[symbol][color];
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
	if (!saiph->world->player.hallucinating && item[symbol]) {
		map<Point, Stash>::iterator s = stashes.find(point);
		if (s != stashes.end()) {
			if ((s->second.top_symbol != symbol || s->second.top_color != color)) {
				/* top symbol/color changed, update */
				if (s->second.top_symbol != ILLEGAL_ITEM)
					s->second.turn_changed = saiph->world->player.turn;
				s->second.top_symbol = symbol;
				s->second.top_color = color;
			}
		} else {
			/* new stash */
			stashes[point] = Stash(saiph->world->player.turn, symbol, color);
		}
	} else if (symbol == dungeonmap[point.row][point.col]) {
		/* if there ever was a stash here, it's gone now */
		stashes.erase(point);
	}

	/* update monsters */
	if (monster[symbol] && point != saiph->position) {
		/* add a monster, or update position of an existing monster */
		unsigned char msymbol;
		if (symbol == INVERSE)
			msymbol = PET;
		else
			msymbol = symbol;
		/* set monster on monstermap */
		monstermap[point.row][point.col] = msymbol;
		/* find nearest monster */
		int min_distance = INT_MAX;
		map<Point, Monster>::iterator nearest = monsters.end();
		for (map<Point, Monster>::iterator m = monsters.begin(); m != monsters.end(); ++m) {
			if (m->second.symbol != msymbol || m->second.color != color)
				continue; // not the same monster
			unsigned char old_symbol;
			if (symbol == INVERSE)
				old_symbol = PET;
			else
				old_symbol = saiph->world->view[m->first.row][m->first.col];
			if (m->second.symbol == old_symbol && m->second.color == saiph->world->color[m->first.row][m->first.col])
				continue; // this monster already is on its square
			/* see if this monster is closer than the last found monster */
			int distance = max(abs(m->first.row - point.row), abs(m->first.col - point.col));
			if (distance >= min_distance)
				continue;
			/* it is */
			min_distance = distance;
			nearest = m;
		}
		if (nearest != monsters.end()) {
			/* we know of this monster, move it to new location */
			/* remove monster from monstermap */
			monstermap[nearest->first.row][nearest->first.col] = ILLEGAL_MONSTER;
			/* update monster */
			monsters[point] = nearest->second;
			monsters.erase(nearest);
		} else {
			/* add monster */
			monsters[point] = Monster(msymbol, color);
		}
	}
}

void Level::updateMonsters() {
	/* remove monsters that seems to be gone
	 * and make monsters we can't see !visible */
	for (map<Point, Monster>::iterator m = monsters.begin(); m != monsters.end(); ) {
		unsigned char symbol;
		if (saiph->world->color[m->first.row][m->first.col] == INVERSE)
			symbol = PET;
		else
			symbol = saiph->world->view[m->first.row][m->first.col];
		/* if we don't see the monster on world->view then it's not visible */
		m->second.visible = (symbol == m->second.symbol && saiph->world->color[m->first.row][m->first.col] == m->second.color);
		if (abs(saiph->position.row - m->first.row) > 1 || abs(saiph->position.col - m->first.col) > 1) {
			/* player is not next to where we last saw the monster */
			++m;
			continue;
		}
		if (m->first != saiph->position && symbol == m->second.symbol && saiph->world->color[m->first.row][m->first.col] == m->second.color) {
			/* we can still see the monster */
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
	/* create pathmap in given map */
	/* first reset nextrow, nextcol, cost & move */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			pathmap[r][c].nextrow = -1;
			pathmap[r][c].nextcol = -1;
			pathmap[r][c].cost = UINT_MAX;
			pathmap[r][c].move = ILLEGAL_DIRECTION;
		}
	}
	Point from = saiph->position;
	pathing_queue[0] = from;
	pathmap[from.row][from.col].cost = 0;
	pathmap[from.row][from.col].move = NOWHERE;
	int curnode = 0;
	int nodes = 1;
	while (curnode < nodes) {
		from = pathing_queue[curnode++];
		/* check northwest node */
		Point to(from.row - 1, from.col - 1);
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = NW;
			pathing_queue[nodes++] = to;
		}
		/* check north node */
		++to.col;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = N;
			pathing_queue[nodes++] = to;
		}
		/* check northeast node */
		++to.col;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = NE;
			pathing_queue[nodes++] = to;
		}
		/* check east node */
		++to.row;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = E;
			pathing_queue[nodes++] = to;
		}
		/* check southeast node */
		++to.row;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = SE;
			pathing_queue[nodes++] = to;
		}
		/* check south node */
		--to.col;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = S;
			pathing_queue[nodes++] = to;
		}
		/* check southwest node */
		--to.col;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = SW;
			pathing_queue[nodes++] = to;
		}
		/* check west node */
		--to.row;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = W;
			pathing_queue[nodes++] = to;
		}
	}
}

/* private methods */
void Level::addItemToStash(const Point &point, const Item &item) {
	Debug::notice() << "Adding " << item.count << " " << item.name << " to stash at " << point.row << ", " << point.col << endl;
	if (item.count <= 0)
		return;
	map<Point, Stash>::iterator s = stashes.find(point);
	if (s != stashes.end()) {
		s->second.items.push_back(item);
		return;
	}
	/* new stash */
	Stash stash(saiph->world->player.turn);
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

bool Level::updatePathMapHelper(const Point &to, const Point &from) {
	/* helper method for updatePathMap()
	 * return true if the move is legal and we should path further from this node */
	if (to.row < MAP_ROW_BEGIN || to.row > MAP_ROW_END || to.col < MAP_COL_BEGIN || to.col > MAP_COL_END)
		return false; // outside map
	unsigned char s = dungeonmap[to.row][to.col];
	if (!passable[s])
		return false;
	unsigned char m = monstermap[to.row][to.col];
	if (monster[m]) {
		if (m == '@') {
			/* we'll allow pathing through unseen priests */
			map<Point, Monster>::iterator mon = monsters.find(to);
			if (mon == monsters.end())
				return false;
			if (mon->second.visible)
				return false;
			if (!mon->second.priest)
				return false;
		} else if (m == PET) {
			/* we'll allow pathing through pets too */
		} else {
			/* all other monsters we won't path through */
			return false;
		}
	}
	if (monster[m] && m != PET)
		return false; // can't path through monsters (except pets)
	bool cardinal_move = (to.row == from.row || to.col == from.col);
	if (!cardinal_move) {
		if (s == OPEN_DOOR || dungeonmap[from.row][from.col] == OPEN_DOOR)
			return false; // diagonally in/out of door
		if (s == UNKNOWN_TILE || dungeonmap[from.row][from.col] == UNKNOWN_TILE)
			return false; // don't know what tile this is, it may be a door. no diagonal movement
		unsigned char sc1 = dungeonmap[to.row][from.col];
		unsigned char sc2 = dungeonmap[from.row][to.col];
		if (!passable[sc1] && !passable[sc2]) {
			/* moving past two corners
			 * while we may pass two corners if we're not carrying too much we'll just ignore this.
			 * it's bound to cause issues */
			if (sc1 != BOULDER && sc2 != BOULDER)
				return false; // neither corner is a boulder, we may not pass
			else if (branch == BRANCH_SOKOBAN)
				return false; // in sokoban we can't pass by boulders diagonally
		}
		//if (polymorphed_to_grid_bug)
		//      return false;
	}
	//if (blacklisted_move)
	//      return false;
	//if (s == LAVA && !levitating)
	//      return false;
	//if (s == WATER && (!levitating || !waterwalk))
	//      return false;
	unsigned int newcost = pathmap[from.row][from.col].cost + (cardinal_move ? COST_CARDINAL : COST_DIAGONAL);
	newcost += pathcost[s];
	newcost += pathcost[m];
	if (newcost < pathmap[to.row][to.col].cost) {
		pathmap[to.row][to.col].nextrow = from.row;
		pathmap[to.row][to.col].nextcol = from.col;
		pathmap[to.row][to.col].cost = newcost;
		return true;
	}
	return false;
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
	/* dungeon symbol tracking */
	track_symbol[(unsigned char) OPEN_DOOR] = true;
	track_symbol[(unsigned char) CLOSED_DOOR] = true;
	track_symbol[(unsigned char) STAIRS_UP] = true;
	track_symbol[(unsigned char) STAIRS_DOWN] = true;
	track_symbol[(unsigned char) ALTAR] = true;
	track_symbol[(unsigned char) GRAVE] = true;
	track_symbol[(unsigned char) THRONE] = true;
	track_symbol[(unsigned char) SINK] = true;
	track_symbol[(unsigned char) FOUNTAIN] = true;
	track_symbol[(unsigned char) VERTICAL_WALL] = true;
	track_symbol[(unsigned char) HORIZONTAL_WALL] = true;
	/* pathing & maps */
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
	passable[(unsigned char) UNKNOWN_TILE_DIAGONALLY_PASSABLE] = true;
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
	passable[(unsigned char) PET] = true;
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
	pathcost[(unsigned char) ICE] = COST_ICE;
	pathcost[(unsigned char) LAVA] = COST_LAVA;
	pathcost[(unsigned char) PET] = COST_PET;
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
}
