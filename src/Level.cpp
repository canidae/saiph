#include "Level.h"

#include <stdlib.h>
#include "Debug.h"
#include "EventBus.h"
#include "Inventory.h"
#include "Item.h"
#include "Saiph.h"
#include "World.h"
#include "Events/ItemsOnGround.h"
#include "Events/ReceivedItems.h"
#include "Events/StashChanged.h"

/* pathing */
#define COST_CARDINAL 2
#define COST_DIAGONAL 3
#define COST_FOUNTAIN 4 // better not fight on fountains
#define COST_GRAVE 4 // better not fight on graves
#define COST_ALTAR 4 // better not fight on altars
#define COST_ICE 8 // slippery and risky, try to find a way around (don't try very hard, though)
#define COST_LAVA 512 // lava, hot!
#define COST_MONSTER 64 // try not to path through monsters
#define COST_TRAP 128 // avoid traps
#define COST_WATER 256 // avoid water if possible
/* max moves a monster can do before we think it's a new monster */
#define MAX_MONSTER_MOVE 3 // if a monster is more than this distance from where we last saw it, then it's probably a new monster
/* debugging that should be moved to Debug in the future */
#define LEVEL_DEBUG_NAME "Level] "
/* messages */
#define LEVEL_ALTAR_HERE "  There is an altar to "
#define LEVEL_FOUNTAIN_DRIES_UP "  The fountain dries up!  "
#define LEVEL_FOUNTAIN_DRIES_UP2 "  As the hand retreats, the fountain disappears!  "
#define LEVEL_FOUNTAIN_HERE "  There is a fountain here.  "
#define LEVEL_GRAVE_HERE "  There is a grave here.  "
#define LEVEL_NO_STAIRS_DOWN_HERE "  You can't go down here.  "
#define LEVEL_NO_STAIRS_UP_HERE "  You can't go up here.  "
#define LEVEL_OPEN_DOOR_HERE "  There is an open door here.  "
#define LEVEL_STAIRCASE_DOWN_HERE "  There is a staircase down here.  "
#define LEVEL_STAIRCASE_UP_HERE "  There is a staircase up here.  "
#define LEVEL_THERE_IS_NOTHING_HERE "  There is nothing here to pick up.  "
#define LEVEL_THINGS_THAT_ARE_HERE "  Things that are here:  "
#define LEVEL_THINGS_THAT_YOU_FEEL_HERE "  Things that you feel here:  "
#define LEVEL_YOU_FEEL_HERE "  You feel here " // not two spaces here as it's followed by eg. "a lichen corpse"
#define LEVEL_YOU_FEEL_NO_OBJECTS "  You feel no objects here.  "
#define LEVEL_YOU_SEE_HERE "  You see here " // not two spaces here as it's followed by eg. "a lichen corpse"
#define LEVEL_YOU_SEE_NO_OBJECTS "  You see no objects here.  "
#define LEVEL_WALL_UNDIGGABLE "wall is too hard to dig into."
#define LEVEL_FLOOR_OR_GROUND_UNDIGGABLE "here is too hard to dig in."

using namespace event;
using namespace std;

/* define static variables */
/* public */
bool Level::_passable[UCHAR_MAX + 1] = {false};
/* private */
Point Level::_pathing_queue[PATHING_QUEUE_SIZE] = {Point()};
int Level::_pathing_queue_size = 0;
unsigned char Level::_uniquemap[UCHAR_MAX + 1][CHAR_MAX + 1] = {
	{0}
};
int Level::_pathcost[UCHAR_MAX + 1] = {0};
bool Level::_dungeon[UCHAR_MAX + 1] = {false};
bool Level::_monster[UCHAR_MAX + 1] = {false};
bool Level::_item[UCHAR_MAX + 1] = {false};
Tile Level::_outside_map;

/* constructors/destructor */
Level::Level(int level, const string& name, int branch) : _level(level), _monsters(), _stashes(), _symbols(), _name(name), _branch(branch), _walls_diggable(true), _floor_diggable(true) {
	for (int a = 0; a < MAP_ROW_END + 1; ++a) {
		for (int b = 0; b < MAP_COL_END + 1; ++b)
			_map[a][b] = Tile(Coordinate(_level, a, b));
	}
	sscanf(name.c_str(), "%*[^0123456789]%d", &_depth);
}

/* public static methods */
bool Level::isPassable(unsigned char symbol) {
	return _passable[symbol];
}

const Tile& Level::outsideMap() {
	return _outside_map;
}

void Level::init() {
	/* monsters */
	for (int a = 0; a <= UCHAR_MAX; ++a) {
		if ((a >= '@' && a <= 'Z') || (a >= 'a' && a <= 'z') || (a >= '1' && a <= '5') || a == '&' || a == '\'' || a == ':' || a == ';' || a == '~' || a == PET)
			_monster[a] = true;
	}
	/* items */
	_item[(unsigned char) WEAPON] = true;
	_item[(unsigned char) ARMOR] = true;
	_item[(unsigned char) RING] = true;
	_item[(unsigned char) AMULET] = true;
	_item[(unsigned char) TOOL] = true;
	_item[(unsigned char) FOOD] = true;
	_item[(unsigned char) POTION] = true;
	_item[(unsigned char) SCROLL] = true;
	_item[(unsigned char) SPELLBOOK] = true;
	_item[(unsigned char) WAND] = true;
	_item[(unsigned char) GOLD] = true;
	_item[(unsigned char) GEM] = true;
	_item[(unsigned char) STATUE] = true;
	// skipping boulder as that's a special item
	_item[(unsigned char) IRON_BALL] = true;
	_item[(unsigned char) CHAINS] = true;
	_item[(unsigned char) VENOM] = true;
	/* stuff we can walk on */
	_passable[(unsigned char) FLOOR] = true;
	_passable[(unsigned char) OPEN_DOOR] = true;
	_passable[(unsigned char) CORRIDOR] = true;
	_passable[(unsigned char) STAIRS_UP] = true;
	_passable[(unsigned char) STAIRS_DOWN] = true;
	_passable[(unsigned char) ALTAR] = true;
	_passable[(unsigned char) GRAVE] = true;
	_passable[(unsigned char) THRONE] = true;
	_passable[(unsigned char) SINK] = true;
	_passable[(unsigned char) FOUNTAIN] = true;
	_passable[(unsigned char) WATER] = true;
	_passable[(unsigned char) ICE] = true;
	_passable[(unsigned char) LAVA] = true;
	_passable[(unsigned char) LOWERED_DRAWBRIDGE] = true;
	_passable[(unsigned char) TRAP] = true;
	_passable[(unsigned char) UNKNOWN_TILE] = true;
	_passable[(unsigned char) UNKNOWN_TILE_DIAGONALLY_UNPASSABLE] = true;
	_passable[(unsigned char) ROGUE_STAIRS] = true;
	_passable[(unsigned char) MINES_FOUNTAIN] = true;
	_passable[(unsigned char) SHOP_TILE] = true;
	_passable[(unsigned char) WEAPON] = true;
	_passable[(unsigned char) ARMOR] = true;
	_passable[(unsigned char) RING] = true;
	_passable[(unsigned char) AMULET] = true;
	_passable[(unsigned char) TOOL] = true;
	_passable[(unsigned char) FOOD] = true;
	_passable[(unsigned char) POTION] = true;
	_passable[(unsigned char) SCROLL] = true;
	_passable[(unsigned char) SPELLBOOK] = true;
	_passable[(unsigned char) WAND] = true;
	_passable[(unsigned char) GOLD] = true;
	_passable[(unsigned char) GEM] = true;
	_passable[(unsigned char) STATUE] = true;
	_passable[(unsigned char) IRON_BALL] = true;
	_passable[(unsigned char) CHAINS] = true;
	_passable[(unsigned char) VENOM] = true;
	/* dungeon layout */
	_dungeon[(unsigned char) VERTICAL_WALL] = true;
	_dungeon[(unsigned char) HORIZONTAL_WALL] = true;
	_dungeon[(unsigned char) FLOOR] = true;
	_dungeon[(unsigned char) OPEN_DOOR] = true;
	_dungeon[(unsigned char) CLOSED_DOOR] = true;
	_dungeon[(unsigned char) IRON_BARS] = true;
	_dungeon[(unsigned char) TREE] = true;
	_dungeon[(unsigned char) CORRIDOR] = true;
	_dungeon[(unsigned char) STAIRS_UP] = true;
	_dungeon[(unsigned char) STAIRS_DOWN] = true;
	_dungeon[(unsigned char) ALTAR] = true;
	_dungeon[(unsigned char) GRAVE] = true;
	_dungeon[(unsigned char) THRONE] = true;
	_dungeon[(unsigned char) SINK] = true;
	_dungeon[(unsigned char) FOUNTAIN] = true;
	_dungeon[(unsigned char) WATER] = true;
	_dungeon[(unsigned char) ICE] = true;
	_dungeon[(unsigned char) LAVA] = true;
	_dungeon[(unsigned char) LOWERED_DRAWBRIDGE] = true;
	_dungeon[(unsigned char) RAISED_DRAWBRIDGE] = true;
	_dungeon[(unsigned char) TRAP] = true;
	_dungeon[(unsigned char) BOULDER] = true; // hardly static, but we won't allow moving on to one
	_dungeon[(unsigned char) ROGUE_STAIRS] = true; // unique, is both up & down stairs
	_dungeon[(unsigned char) MINES_FOUNTAIN] = true; // unique, but [mostly] static
	_dungeon[(unsigned char) SHOP_TILE] = true; // unique, but [mostly] static
	/* cost for pathing on certain tiles */
	_pathcost[(unsigned char) FOUNTAIN] = COST_FOUNTAIN;
	_pathcost[(unsigned char) GRAVE] = COST_GRAVE;
	_pathcost[(unsigned char) ALTAR] = COST_ALTAR;
	_pathcost[(unsigned char) ICE] = COST_ICE;
	_pathcost[(unsigned char) LAVA] = COST_LAVA;
	_pathcost[(unsigned char) MINES_FOUNTAIN] = COST_FOUNTAIN;
	_pathcost[(unsigned char) TRAP] = COST_TRAP;
	_pathcost[(unsigned char) WATER] = COST_WATER;
	/* remapping ambigous symbols */
	for (int s = 0; s <= UCHAR_MAX; ++s) {
		for (int c = 0; c <= CHAR_MAX; ++c)
			_uniquemap[s][c] = s;
	}
	_uniquemap[(unsigned char) CORRIDOR][CYAN] = IRON_BARS;
	_uniquemap[(unsigned char) CORRIDOR][GREEN] = TREE;
	_uniquemap[(unsigned char) FLOOR][CYAN] = ICE;
	_uniquemap[(unsigned char) FLOOR][YELLOW] = LOWERED_DRAWBRIDGE;
	_uniquemap[(unsigned char) FOUNTAIN][NO_COLOR] = SINK;
	_uniquemap[(unsigned char) GRAVE][YELLOW] = THRONE;
	_uniquemap[(unsigned char) HORIZONTAL_WALL][YELLOW] = OPEN_DOOR;
	_uniquemap[(unsigned char) VERTICAL_WALL][YELLOW] = OPEN_DOOR;
	_uniquemap[(unsigned char) WATER][RED] = LAVA;
	_uniquemap[(unsigned char) TRAP][BOLD_MAGENTA] = MAGIC_PORTAL;
}

void Level::destroy() {
}

/* public methods */
int Level::branch() const {
	return _branch;
}

int Level::branch(int branch) {
	_branch = branch;
	return this->branch();
}

int Level::depth() const {
	return _depth;
}

const string& Level::name() const {
	return _name;
}

Tile& Level::tile() {
	if (!Saiph::position().insideMap())
		return _outside_map;
	return _map[Saiph::position().row()][Saiph::position().col()];
}

Tile& Level::tile(const Point& point) {
	if (!point.insideMap())
		return _outside_map;
	return _map[point.row()][point.col()];
}

const map<Point, Monster>& Level::monsters() const {
	return _monsters;
}

const map<Point, Stash>& Level::stashes() const {
	return _stashes;
}

const map<Point, int>& Level::symbols(unsigned char symbol) const {
	return _symbols[symbol];
}

void Level::analyze() {
	if (Saiph::engulfed()) {
		/* we'll still need to update monster's "visible" while engulfed,
		 * or she may attempt to farlook a monster she can't see */
		for (map<Point, Monster>::iterator m = _monsters.begin(); m != _monsters.end(); ++m)
			m->second.visible(false);
		return;
	}
	/* update changed symbols */
	for (vector<Point>::const_iterator c = World::changes().begin(); c != World::changes().end(); ++c)
		updateMapPoint(*c, (unsigned char) World::view(*c), World::color(*c));
	/* update monsters */
	updateMonsters();
	/* update pathmap */
	updatePathMap();
	/* set point we're standing on "fully searched" */
	_map[Saiph::position().row()][Saiph::position().col()].search(TILE_FULLY_SEARCHED);

	/* send event if we're standing on stash */
	map<Point, Stash>::iterator s = _stashes.find(Saiph::position());
	if (s != _stashes.end() && s->second.items().size() > 0) {
		/* broadcast "ItemsOnGround" */
		ItemsOnGround on_ground(s->second.items());
		EventBus::broadcast(static_cast<Event*> (&on_ground));
	}
}

void Level::parseMessages(const string& messages) {
	/* parse messages that can help us find doors/staircases/etc. */
	string::size_type pos;
	if (messages.find(LEVEL_STAIRCASE_UP_HERE) != string::npos) {
		setDungeonSymbol(Saiph::position(), STAIRS_UP);
	} else if (messages.find(LEVEL_STAIRCASE_DOWN_HERE) != string::npos) {
		setDungeonSymbol(Saiph::position(), STAIRS_DOWN);
	} else if (messages.find(LEVEL_OPEN_DOOR_HERE) != string::npos) {
		setDungeonSymbol(Saiph::position(), OPEN_DOOR);
	} else if (messages.find(LEVEL_GRAVE_HERE) != string::npos) {
		setDungeonSymbol(Saiph::position(), GRAVE);
	} else if (messages.find(LEVEL_FOUNTAIN_HERE) != string::npos) {
		setDungeonSymbol(Saiph::position(), FOUNTAIN);
	} else if (messages.find(LEVEL_FOUNTAIN_DRIES_UP) != string::npos || messages.find(LEVEL_FOUNTAIN_DRIES_UP2) != string::npos) {
		setDungeonSymbol(Saiph::position(), FLOOR);
	} else if (messages.find(LEVEL_NO_STAIRS_DOWN_HERE) != string::npos || messages.find(LEVEL_NO_STAIRS_UP_HERE) != string::npos) {
		setDungeonSymbol(Saiph::position(), UNKNOWN_TILE);
	} else if ((pos = messages.find(LEVEL_ALTAR_HERE)) != string::npos) {
		setDungeonSymbol(Saiph::position(), ALTAR);
		/* set symbol value too */
		if (messages.find(" (lawful) ", pos) != string::npos)
			_symbols[(unsigned char) ALTAR][Saiph::position()] = LAWFUL;
		else if (messages.find(" (neutral) ", pos) != string::npos)
			_symbols[(unsigned char) ALTAR][Saiph::position()] = NEUTRAL;
		else if (messages.find(" (chaotic) ", pos) != string::npos)
			_symbols[(unsigned char) ALTAR][Saiph::position()] = CHAOTIC;
		else
			_symbols[(unsigned char) ALTAR][Saiph::position()] = UNALIGNED;
	} else if (messages.find(LEVEL_WALL_UNDIGGABLE) != string::npos) {
		_walls_diggable = false;
	} else if (messages.find(LEVEL_FLOOR_OR_GROUND_UNDIGGABLE) != string::npos) {
		_floor_diggable = false;
	}

	/* figure out if there's something on the ground */
	if ((pos = messages.find(LEVEL_YOU_SEE_HERE)) != string::npos) {
		/* we see a single item on ground */
		map<Point, Stash>::iterator s = _stashes.find(Saiph::position());
		if (s != _stashes.end())
			s->second.items().clear(); // clear stash items
		else
			s = _stashes.insert(s, make_pair(Saiph::position(), Stash())); // no stash at location, create one
		s->second.lastInspected(World::turn());
		pos += sizeof (LEVEL_YOU_SEE_HERE) - 1;
		string::size_type length = messages.find(".  ", pos);
		if (length != string::npos) {
			length = length - pos;
			Item item(messages.substr(pos, length));
			if (item.count() > 0)
				s->second.items().push_back(item);
		}
	} else if ((pos = messages.find(LEVEL_YOU_FEEL_HERE)) != string::npos) {
		/* we feel a single item on the ground */
		map<Point, Stash>::iterator s = _stashes.find(Saiph::position());
		if (s != _stashes.end())
			s->second.items().clear(); // clear stash items
		else
			s = _stashes.insert(s, make_pair(Saiph::position(), Stash())); // no stash at location, create one
		s->second.lastInspected(World::turn());
		pos += sizeof (LEVEL_YOU_FEEL_HERE) - 1;
		string::size_type length = messages.find(".  ", pos);
		if (length != string::npos) {
			length = length - pos;
			Item item(messages.substr(pos, length));
			if (item.count() > 0)
				s->second.items().push_back(item);
		}
	} else if ((pos = messages.find(LEVEL_THINGS_THAT_ARE_HERE)) != string::npos || (pos = messages.find(LEVEL_THINGS_THAT_YOU_FEEL_HERE)) != string::npos) {
		/* we see/feel multiple items on the ground */
		map<Point, Stash>::iterator s = _stashes.find(Saiph::position());
		if (s != _stashes.end())
			s->second.items().clear(); // clear stash items
		else
			s = _stashes.insert(s, make_pair(Saiph::position(), Stash())); // no stash at location, create one
		s->second.lastInspected(World::turn());
		pos = messages.find("  ", pos + 1);
		while (pos != string::npos && messages.size() > pos + 2) {
			pos += 2;
			string::size_type length = messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			Item item(messages.substr(pos, length));
			if (item.count() > 0)
				s->second.items().push_back(item);
			pos += length;
		}
	} else if (messages.find(LEVEL_YOU_SEE_NO_OBJECTS) != string::npos || messages.find(LEVEL_YOU_FEEL_NO_OBJECTS) != string::npos || messages.find(LEVEL_THERE_IS_NOTHING_HERE) != string::npos) {
		/* we see/feel no items on the ground */
		_stashes.erase(Saiph::position());
	}
}

void Level::setDirtyStash(const Point& point) {
	map<Point, Stash>::iterator s = _stashes.find(point);
	if (s != _stashes.end())
		s->second.items().clear();
}

void Level::setDungeonSymbol(const Point& point, unsigned char symbol) {
	/* need to update both _map and _symbols,
	 * better keep it in a method */
	if (!point.insideMap())
		return;
	Tile& t = _map[point.row()][point.col()];
	if (t.symbol() == symbol)
		return; // no change
	/* erase old symbol from symbols */
	_symbols[t.symbol()].erase(point);
	/* set new symbol in symbols */
	_symbols[symbol][point] = UNKNOWN_SYMBOL_VALUE;
	/* update tile in _map */
	t.symbol(symbol);
}

void Level::setDungeonSymbolValue(const Point& point, int value) {
	if (!point.insideMap())
		return;
	_symbols[_map[point.row()][point.col()].symbol()][point] = value;
}

void Level::setMonster(const Point& point, const Monster& monster) {
	if (!point.insideMap())
		return;
	_monsters[point] = monster;
}

void Level::increaseAdjacentSearchCount(const Point& point) {
	/* increase search count for adjacent points to given point */
	Point p = point;
	for (p.row(point.row() - 1); p.row() <= point.row() + 1; p.moveSouth()) {
		for (p.col(point.col() - 1); p.col() <= point.col() + 1; p.moveEast()) {
			if (!p.insideMap())
				continue;
			_map[p.row()][p.col()].searchInc();
		}
	}
}

/* private methods */
void Level::updateMapPoint(const Point& point, unsigned char symbol, int color) {
	Tile& t = _map[point.row()][point.col()];
	if (_branch == BRANCH_ROGUE) {
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
			if (t.symbol() != STAIRS_DOWN && t.symbol() != STAIRS_UP)
				symbol = ROGUE_STAIRS;
			else
				symbol = t.symbol();
			break;

		default:
			break;
		}
	} else {
		/* remap ambigous symbols */
		symbol = _uniquemap[symbol][color];
		/* some special cases */
		if (symbol == FOUNTAIN && _branch == BRANCH_MINES)
			symbol = MINES_FOUNTAIN; // to avoid dipping & such
		else if (symbol == FLOOR && t.symbol() == SHOP_TILE)
			symbol = SHOP_TILE;
	}
	if (_dungeon[symbol] || (symbol == SOLID_ROCK && t.symbol() == CORRIDOR)) {
		/* update the map showing static stuff, also forgets disappearing corridors (ie. following guard out of vault) */
		setDungeonSymbol(point, symbol);
	} else if (symbol != SOLID_ROCK && !_passable[t.symbol()] && t.symbol() != UNKNOWN_TILE_UNPASSABLE) {
		/* we can't see the floor here, but we believe we can pass this tile.
		 * place an UNKNOWN_TILE here.
		 * the reason we check if stored tile is !passable is because if we don't,
		 * then every tile a monster steps on or drops an item on will become UNKNOWN_TILE,
		 * even if we already know what's beneath the monster/item. */
		setDungeonSymbol(point, UNKNOWN_TILE);
	}
	/* update items */
	if (!Saiph::hallucinating() && _item[symbol]) {
		map<Point, Stash>::iterator s = _stashes.find(point);
		if (s != _stashes.end()) {
			if ((s->second.symbol() != symbol || s->second.color() != color)) {
				/* top symbol/color changed, update */
				s->second.symbol(symbol);
				s->second.color(color);
				/* broadcast StashChanged */
				StashChanged sc(Coordinate(Saiph::position().level(), point));
				EventBus::broadcast(static_cast<Event*> (&sc));
			}
		} else {
			/* new stash */
			_stashes[point] = Stash(symbol, color);
			/* broadcast StashChanged */
			StashChanged sc(Coordinate(Saiph::position().level(), point));
			EventBus::broadcast(static_cast<Event*> (&sc));
		}
	} else if (symbol == t.symbol()) {
		/* if there ever was a stash here, it's gone now */
		_stashes.erase(point);
	}

	/* update monsters */
	if (_monster[symbol] && point != Saiph::position()) {
		/* add a monster, or update position of an existing monster */
		unsigned char msymbol;
		if ((color >= INVERSE_BLACK && color <= INVERSE_WHITE) || (color >= INVERSE_BOLD_BLACK && color <= INVERSE_BOLD_WHITE))
			msymbol = PET;
		else
			msymbol = symbol;
		/* find nearest monster */
		int min_distance = INT_MAX;
		map<Point, Monster>::iterator nearest = _monsters.end();
		for (map<Point, Monster>::iterator m = _monsters.begin(); m != _monsters.end(); ++m) {
			if (m->second.symbol() != msymbol || m->second.color() != color)
				continue; // not the same monster
			unsigned char old_symbol;
			if ((color >= INVERSE_BLACK && color <= INVERSE_WHITE) || (color >= INVERSE_BOLD_BLACK && color <= INVERSE_BOLD_WHITE))
				old_symbol = PET;
			else
				old_symbol = World::view(m->first);
			if (m->second.symbol() == old_symbol && m->second.color() == World::color(m->first)) {
				/* note about this "point == m->first":
				 * the character for the monster may be updated even if it hasn't moved,
				 * if this is the case, we should return and neither move nor add the
				 * monster as that will screw up the data we know about the monster */
				if (point == m->first)
					return;
				continue; // this monster already is on its square
			}
			/* see if this monster is closer than the last found monster */
			int distance = max(abs(m->first.row() - point.row()), abs(m->first.col() - point.col()));
			if (distance > MAX_MONSTER_MOVE)
				continue; // too far away from where we last saw it, probably new monster
			else if (distance >= min_distance)
				continue;
			else if ((m->second.priest() || m->second.shopkeeper()) && distance > 1)
				continue; // shopkeepers and priests are very close to eachother in minetown
			/* it is */
			min_distance = distance;
			nearest = m;
		}
		if (nearest != _monsters.end()) {
			/* we know of this monster, move it to new location */
			/* remove monster from _map */
			_map[nearest->first.row()][nearest->first.col()].monster(ILLEGAL_MONSTER);
			/* update monster */
			nearest->second.lastSeen(World::turn());
			_monsters[point] = nearest->second;
			_monsters.erase(nearest);
		} else {
			/* add monster */
			_monsters[point] = Monster(msymbol, color, World::turn());
		}
		/* set monster on tile */
		t.monster(msymbol);
	}
}

void Level::updateMonsters() {
	/* remove monsters that seems to be gone
	 * and make monsters we can't see !visible */
	for (map<Point, Monster>::iterator m = _monsters.begin(); m != _monsters.end();) {
		unsigned char symbol;
		int color = World::color(m->first);
		if ((color >= INVERSE_BLACK && color <= INVERSE_WHITE) || (color >= INVERSE_BOLD_BLACK && color <= INVERSE_BOLD_WHITE))
			symbol = PET;
		else
			symbol = World::view(m->first);
		/* if we don't see the monster on world->view then it's not visible */
		m->second.visible((m->first != Saiph::position() && symbol == m->second.symbol() && color == m->second.color()));
		if (m->second.visible()) {
			/* monster still visible, don't remove it */
			++m;
			continue;
		} else if (abs(Saiph::position().row() - m->first.row()) > 1 || abs(Saiph::position().col() - m->first.col()) > 1) {
			/* player is not next to where we last saw the monster */
			++m;
			continue;
		}
		/* remove monster from _map */
		_map[m->first.row()][m->first.col()].monster(ILLEGAL_MONSTER);
		/* remove monster from list */
		_monsters.erase(m++);
	}
}

void Level::updatePathMap() {
	Point from = Saiph::position();
	Point to = from;
	_pathing_queue_size = 0;

	/* set node we're standing on */
	_map[from.row()][from.col()].updatePath(Point(), NOWHERE, 0, 0);

	/* check first northwest node */
	updatePathMapSetCost(to.moveNorthwest(), from, NW, 0);
	/* check first north node */
	updatePathMapSetCost(to.moveEast(), from, N, 0);
	/* check first northeast node */
	updatePathMapSetCost(to.moveEast(), from, NE, 0);
	/* check first east node */
	updatePathMapSetCost(to.moveSouth(), from, E, 0);
	/* check first southeast node */
	updatePathMapSetCost(to.moveSouth(), from, SE, 0);
	/* check first south node */
	updatePathMapSetCost(to.moveWest(), from, S, 0);
	/* check first southwest node */
	updatePathMapSetCost(to.moveWest(), from, SW, 0);
	/* check first west node */
	updatePathMapSetCost(to.moveNorth(), from, W, 0);

	/* calculate remaining nodes */
	int index = 0;
	while (index < _pathing_queue_size) {
		from = _pathing_queue[index++];
		Point to = from;

		/* previous tile, the tile we came from */
		Tile& prev = _map[from.row()][from.col()];

		/* check northwest node */
		updatePathMapSetCost(to.moveNorthwest(), from, prev.direction(), prev.distance());
		/* check north node */
		updatePathMapSetCost(to.moveEast(), from, prev.direction(), prev.distance());
		/* check northeast node */
		updatePathMapSetCost(to.moveEast(), from, prev.direction(), prev.distance());
		/* check east node */
		updatePathMapSetCost(to.moveSouth(), from, prev.direction(), prev.distance());
		/* check southeast node */
		updatePathMapSetCost(to.moveSouth(), from, prev.direction(), prev.distance());
		/* check south node */
		updatePathMapSetCost(to.moveWest(), from, prev.direction(), prev.distance());
		/* check southwest node */
		updatePathMapSetCost(to.moveWest(), from, prev.direction(), prev.distance());
		/* check west node */
		updatePathMapSetCost(to.moveNorth(), from, prev.direction(), prev.distance());
	}
}

unsigned int Level::updatePathMapCalculateCost(const Point& to, const Point& from) {
	/* helper method for updatePathMap()
	 * return UNREACHABLE if move is illegal, or the cost for reaching the node if move is legal */
	Tile& next = _map[to.row()][to.col()];
	Tile& prev = _map[from.row()][from.col()];
	if (!_passable[next.symbol()])
		return UNREACHABLE;
	bool cardinal_move = (to.row() == from.row() || to.col() == from.col());
	if (!cardinal_move) {
		unsigned char prev_symbol = prev.symbol();
		if (next.symbol() == OPEN_DOOR || prev_symbol == OPEN_DOOR)
			return UNREACHABLE; // diagonally in/out of door
		if (next.symbol() == UNKNOWN_TILE_DIAGONALLY_UNPASSABLE || prev_symbol == UNKNOWN_TILE_DIAGONALLY_UNPASSABLE)
			return UNREACHABLE; // don't know what tile this is, but we know we can't pass it diagonally
		Tile& sc1 = _map[to.row()][from.col()];
		Tile& sc2 = _map[from.row()][to.col()];
		if (!_passable[sc1.symbol()] && !_passable[sc2.symbol()]) {
			/* moving past two corners
			 * while we may pass two corners if we're not carrying too much we'll just ignore this.
			 * it's bound to cause issues */
			if (sc1.symbol() != BOULDER && sc2.symbol() != BOULDER)
				return UNREACHABLE; // neither corner is a boulder, we may not pass
			else if (_branch == BRANCH_SOKOBAN)
				return UNREACHABLE; // in sokoban we can't pass by boulders diagonally
		}
		//if (polymorphed_to_grid_bug)
		//return UNREACHABLE;
	}
	if (next.symbol() == LAVA) // && !levitating)
		return UNREACHABLE;
	if (next.symbol() == WATER) // && (!levitating || !waterwalk))
		return UNREACHABLE;
	if (next.symbol() == TRAP && _branch == BRANCH_SOKOBAN)
		return UNREACHABLE;
	if (next.monster() != ILLEGAL_MONSTER && abs(Saiph::position().row() - to.row()) <= 1 && abs(Saiph::position().col() - to.col()) <= 1)
		return UNREACHABLE; // don't path through monster next to her
	unsigned int cost = prev.cost() + (cardinal_move ? COST_CARDINAL : COST_DIAGONAL);
	cost += _pathcost[next.symbol()];
	if (next.monster() != ILLEGAL_MONSTER)
		cost += COST_MONSTER;
	return cost;
}

void Level::updatePathMapSetCost(const Point& to, const Point& from, unsigned char direction, unsigned int distance) {
	if (!to.insideMap())
		return;
	unsigned int cost = updatePathMapCalculateCost(to, from);
	Tile& next = _map[to.row()][to.col()];
	if (cost < next.cost()) {
		_pathing_queue[_pathing_queue_size++] = to;
		next.updatePath(from, direction, distance + 1, cost);
	} else if (cost == next.cost() && cost == UNREACHABLE) {
		next.updatePath(from, direction, distance + 1, UNPASSABLE);
	}
}
