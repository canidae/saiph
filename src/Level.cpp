#include "Level.h"

#include <stdlib.h>
#include "Debug.h"
#include "EventBus.h"
#include "Inventory.h"
#include "Item.h"
#include "Saiph.h"
#include "World.h"
#include "Data/Monster.h"
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
#define COST_MONSTER 128 // try not to path through monsters
#define COST_TRAP 128 // avoid traps
#define COST_WATER 256 // avoid water if possible
/* max moves a monster can do before we think it's a new monster */
#define MAX_MONSTER_MOVE 3 // if a monster is more than this distance from where we last saw it, then it's probably a new monster
/* debugging that should be moved to Debug in the future */
#define LEVEL_DEBUG_NAME "Level] "
/* messages */
#define LEVEL_ALTAR_HERE "  There is an altar to "
#define LEVEL_ARROW_TRAP_ESCAPE "  You escape an arrow trap.  "
#define LEVEL_ARROW_TRAP_HERE "  There is an arrow trap here.  "
#define LEVEL_ARROW_TRAP_TRIGGER "  An arrow shoots out at you!  "
#define LEVEL_BEAR_TRAP_ESCAPE "  You escape a bear trap.  "
#define LEVEL_BEAR_TRAP_FLOAT "  You float over a bear trap.  "
#define LEVEL_BEAR_TRAP_HERE "  There is a bear trap here.  "
#define LEVEL_BEAR_TRAP_TRIGGER "  A bear trap closes on your foot!  "
#define LEVEL_DART_TRAP_ESCAPE "  You escape a dart trap.  "
#define LEVEL_DART_TRAP_HERE "  There is a dart trap here.  "
#define LEVEL_DART_TRAP_TRIGGER "  A little dart shoots out at you!  "
#define LEVEL_FALLING_ROCK_TRAP_ESCAPE "  You escape a falling rock trap.  "
#define LEVEL_FALLING_ROCK_TRAP_HERE "  There is a falling rock trap here.  "
#define LEVEL_FALLING_ROCK_TRAP_TRIGGER "  A trap door in the ceiling opens and a rock falls on your head!  "
#define LEVEL_FIRE_TRAP_ESCAPE "  You escape a fire trap.  "
#define LEVEL_FIRE_TRAP_HERE "  There is a fire trap here.  "
#define LEVEL_FIRE_TRAP_TRIGGER "  A tower of flame erupts from the floor!  "
#define LEVEL_HOLE_ESCAPE "  You escape a hole.  "
#define LEVEL_HOLE_FLOAT "  You float over a hole.  "
#define LEVEL_FOUNTAIN_DRIES_UP "  The fountain dries up!  "
#define LEVEL_FOUNTAIN_DRIES_UP2 "  As the hand retreats, the fountain disappears!  "
#define LEVEL_FOUNTAIN_HERE "  There is a fountain here.  "
#define LEVEL_GRAVE_HERE "  There is a grave here.  "
#define LEVEL_LAND_MINE_ESCAPE "  You escape a land mine.  "
#define LEVEL_LAND_MINE_FLOAT "  There is a trigger in a pile of soil below you.  "
#define LEVEL_LAND_MINE_HERE "  There is a land mine here.  "
#define LEVEL_LEVEL_TELEPORTER_TRAP_ESCAPE "  You escape a level teleporter trap.  "
#define LEVEL_MAGIC_TRAP_ESCAPE "  You escape a magic trap.  "
#define LEVEL_MAGIC_TRAP_HERE "  There is a magic trap here.  "
// TODO: trap detection with levitation, holes, trap doors, pit, web, squeaky board, etc
#define LEVEL_NO_STAIRS_DOWN_HERE "  You can't go down here.  "
#define LEVEL_NO_STAIRS_UP_HERE "  You can't go up here.  "
#define LEVEL_OPEN_DOOR_HERE "  There is an open door here.  "
#define LEVEL_POLYMORPH_TRAP_ESCAPE "  You escape a polymorph trap.  "
#define LEVEL_ROLLING_BOULDER_TRAP_ESCAPE "  You escape a rolling boulder trap.  "
#define LEVEL_ROLLING_BOULDER_TRAP_HERE "  There is a rolling boulder trap here.  "
#define LEVEL_ROLLING_BOULDER_TRAP_TRIGGER "  Click! You trigger a rolling boulder trap!  "
#define LEVEL_RUST_TRAP_ESCAPE "  You escape a rust trap.  "
#define LEVEL_RUST_TRAP_HERE "  There is a rust trap here.  "
#define LEVEL_RUST_TRAP_TRIGGER "  A gush of water hits "
#define LEVEL_SLEEPING_GAS_TRAP_ESCAPE "  You escape a sleeping gas trap.  "
#define LEVEL_SLEEPING_GAS_TRAP_HERE "  There is a sleeping gas trap here.  "
#define LEVEL_SLEEPING_GAS_TRAP_TRIGGER "  A cloud of gas puts you to sleep!  "
#define LEVEL_SQUEAKY_BOARD_ESCAPE "  You escape a squeaky board.  "
#define LEVEL_SQUEAKY_BOARD_FLOAT "  You notice a loose board below you.  "
#define LEVEL_SQUEAKY_BOARD_HERE "  There is a squeaky board here.  "
#define LEVEL_SQUEAKY_BOARD_TRIGGER "  A board beneath you squeaks loudly.  "
#define LEVEL_STAIRCASE_DOWN_HERE "  There is a staircase down here.  "
#define LEVEL_STAIRCASE_UP_HERE "  There is a staircase up here.  "
#define LEVEL_PIT_ESCAPE "  You escape a pit.  "
#define LEVEL_PIT_FLOAT "  You float over a pit.  "
#define LEVEL_PIT_HERE "  There is a pit here.  "
#define LEVEL_SPIKED_PIT_ESCAPE "  You escape a spiked pit.  "
#define LEVEL_SPIKED_PIT_FLOAT "  You float over a spiked pit.  "
#define LEVEL_SPIKED_PIT_HERE "  There is a spiked pit here.  "
#define LEVEL_TELEPORTATION_TRAP_ESCAPE "  You escape a teleportation trap.  "
#define LEVEL_TELEPORTATION_TRAP_HERE "  There is a teleportation trap here.  "
#define LEVEL_TRAP_DOOR_ESCAPE "  You escape a trap door.  "
#define LEVEL_TRAP_DOOR_FLOAT "  You don't fall in.  "
#define LEVEL_THERE_IS_NOTHING_HERE "  There is nothing here to pick up.  "
#define LEVEL_THINGS_THAT_ARE_HERE "  Things that are here:  "
#define LEVEL_THINGS_THAT_YOU_FEEL_HERE "  Things that you feel here:  "
#define LEVEL_YOU_FEEL_HERE "  You feel here " // not two spaces here as it's followed by eg. "a lichen corpse"
#define LEVEL_YOU_FEEL_NO_OBJECTS "  You feel no objects here.  "
#define LEVEL_YOU_SEE_HERE "  You see here " // not two spaces here as it's followed by eg. "a lichen corpse"
#define LEVEL_YOU_SEE_NO_OBJECTS "  You see no objects here.  "
#define LEVEL_WALL_UNDIGGABLE "wall is too hard to dig into."
#define LEVEL_FLOOR_OR_GROUND_UNDIGGABLE "here is too hard to dig in."
#define LEVEL_SHOP_WALL_UNDIGGABLE "  This wall seems too hard to dig into.  "
#define LEVEL_SHOP_ON_LEVEL1 "  You hear someone cursing shoplifters.  "
#define LEVEL_SHOP_ON_LEVEL2 "  You hear the chime of a cash register.  "
#define LEVEL_SHOP_ON_LEVEL3 "  You hear Neiman and Marcus arguing!  "
#define LEVEL_WEB_ESCAPE "  You escape a web.  "
#define LEVEL_WEB_FLOAT "  You float over a web.  "
#define LEVEL_WEB_HERE "  There is a web here.  "
#define LEVEL_WEB_TRIGGER "  You stumble into a spider web!  "

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
map<Point,string> Level::_turn_farlooks;
unsigned Level::_farlooked_turn = (unsigned)-1;

/* constructors/destructor */
Level::Level(int level, const string& name, int branch) : _level(level), _monsters(), _stashes(), _symbols(), _name(name), _branch(branch), _walls_diggable(true), _floor_diggable(true), _new_level(true) {
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
	_passable[(unsigned char) MAGIC_PORTAL] = true;
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
	/* roughly speaking, _dungeon is the types that we should continue to remember after they are obscured */
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
	_dungeon[(unsigned char) MAGIC_PORTAL] = true;
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

int Level::identifier() const {
	return _level;
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

const map<Point, Monster*>& Level::monsters() const {
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
		for (map<Point, Monster*>::iterator m = _monsters.begin(); m != _monsters.end(); ++m)
			m->second->visible(false);
		return;
	}
	/* update changed symbols */
	for (vector<Point>::const_iterator c = World::changes().begin(); c != World::changes().end(); ++c)
		updateMapPoint(*c, (unsigned char) World::view(*c), World::color(*c));
	World::forgetChanges();
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
		EventBus::broadcast(&on_ground);
	}

	/* branches we will recognize immediately */
	if (_new_level) {
		if (branch() == BRANCH_MAIN && World::branchCoordinate(BRANCH_SOKOBAN).level() == -1 && depth() >= 5 && depth() <= 9) {
			/* look for sokoban */
			if (tile(Point(8, 37)).symbol() == BOULDER && tile(Point(8, 38)).symbol() == BOULDER && tile(Point(8, 43)).symbol() == BOULDER) {
				/* sokoban 1a */
				Debug::notice() << "Found Sokoban level 1a: " << Saiph::position() << endl;
				branch(BRANCH_SOKOBAN);
			} else if (tile(Point(8, 34)).symbol() == BOULDER && tile(Point(8, 42)).symbol() == BOULDER && tile(Point(9, 34)).symbol() == BOULDER) {
				/* sokoban 1b */
				Debug::notice() << "Found Sokoban level 1b: " << Saiph::position() << endl;
				branch(BRANCH_SOKOBAN);
			}

		}
		if (branch() == BRANCH_MAIN && World::branchCoordinate(BRANCH_ROGUE).level() == -1 && World::view(Point(STATUS_ROW, 8)) == '*') {
			/* rogue level, set branch */
			Debug::notice() << "Found the rogue level: " << Saiph::position() << endl;
			branch(BRANCH_ROGUE);
		}
		if (name().find("Home") != string::npos) {
			Debug::notice() << "Found a quest level: " << Saiph::position() << endl;
			branch(BRANCH_QUEST);
		}
		_new_level = false;
	}
	/* branches we won't immediately recognize */
	if (branch() == BRANCH_MAIN && World::branchCoordinate(BRANCH_MINES).level() == -1 && depth() >= 3 && depth() <= 5) {
		/* if mines are not found and depth is between 3 & 5, we should check if we're in the mines */
		for (map<Point, int>::const_iterator hw = symbols((unsigned char) HORIZONTAL_WALL).begin(); hw != symbols((unsigned char) HORIZONTAL_WALL).end(); ++hw) {
			if (hw->first.row() <= MAP_ROW_BEGIN || hw->first.row() >= MAP_ROW_END || hw->first.col() <= MAP_COL_BEGIN || hw->first.col() >= MAP_COL_END)
				continue;
			/* if we see horizontal walls adjacent to this point (except west & east),
			 * then we're in the mines */
			if (tile(Point(hw->first.row() - 1, hw->first.col() - 1)).symbol() == HORIZONTAL_WALL || tile(Point(hw->first.row() - 1, hw->first.col())).symbol() == HORIZONTAL_WALL || tile(Point(hw->first.row() - 1, hw->first.col() + 1)).symbol() == HORIZONTAL_WALL || tile(Point(hw->first.row() + 1, hw->first.col() - 1)).symbol() == HORIZONTAL_WALL || tile(Point(hw->first.row() + 1, hw->first.col())).symbol() == HORIZONTAL_WALL || tile(Point(hw->first.row() + 1, hw->first.col() + 1)).symbol() == HORIZONTAL_WALL) {
				/* we're in the mines */
				Debug::notice() << "Found the mines: " << Saiph::position() << endl;
				branch(BRANCH_MINES);
				break;
			}
		}
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
	} else if (messages.find(LEVEL_NO_STAIRS_DOWN_HERE) != string::npos) {
		if (tile(Saiph::position()).symbol() == STAIRS_DOWN)
			setDungeonSymbol(Saiph::position(), UNKNOWN_TILE);
	} else if (messages.find(LEVEL_NO_STAIRS_UP_HERE) != string::npos) {
		if (tile(Saiph::position()).symbol() == STAIRS_UP)
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
	} else if (messages.find(LEVEL_ARROW_TRAP_ESCAPE) != string::npos || messages.find(LEVEL_ARROW_TRAP_HERE) != string::npos || messages.find(LEVEL_ARROW_TRAP_TRIGGER) != string::npos) {
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_ARROW);
	} else if (messages.find(LEVEL_BEAR_TRAP_ESCAPE) != string::npos || messages.find(LEVEL_BEAR_TRAP_FLOAT) != string::npos || messages.find(LEVEL_BEAR_TRAP_TRIGGER) != string::npos || messages.find(LEVEL_BEAR_TRAP_HERE) != string::npos) {
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_BEAR);
	} else if (messages.find(LEVEL_DART_TRAP_ESCAPE) != string::npos || messages.find(LEVEL_DART_TRAP_HERE) != string::npos || messages.find(LEVEL_DART_TRAP_TRIGGER) != string::npos) {
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_DART);
	} else if (messages.find(LEVEL_FALLING_ROCK_TRAP_ESCAPE) != string::npos || messages.find(LEVEL_FALLING_ROCK_TRAP_HERE) != string::npos || messages.find(LEVEL_FALLING_ROCK_TRAP_TRIGGER) != string::npos) {
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_FALLING_ROCK);
	} else if (messages.find(LEVEL_FIRE_TRAP_ESCAPE) != string::npos || messages.find(LEVEL_FIRE_TRAP_HERE) != string::npos) {
		/* can't use message on trigger, the same message may come from a magic trap */
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_FIRE);
	} else if (messages.find(LEVEL_HOLE_ESCAPE) != string::npos || messages.find(LEVEL_HOLE_FLOAT) != string::npos) {
		/* can't use messages for falling in, by the time we read the message, we're somewhere else */
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_HOLE);
	} else if (messages.find(LEVEL_LAND_MINE_ESCAPE) != string::npos || messages.find(LEVEL_LAND_MINE_FLOAT) != string::npos || messages.find(LEVEL_LAND_MINE_HERE) != string::npos) {
		/* no need to parse message when trap triggered, it'll be gone then */
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_LAND_MINE);
	} else if (messages.find(LEVEL_LEVEL_TELEPORTER_TRAP_ESCAPE) != string::npos) {
		/* level teleporter traps disappear on use */
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_LEVEL_TELEPORTER);
	} else if (messages.find(LEVEL_MAGIC_TRAP_ESCAPE) != string::npos || messages.find(LEVEL_MAGIC_TRAP_HERE) != string::npos) {
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_MAGIC);
	} else if (messages.find(LEVEL_PIT_ESCAPE) != string::npos  || messages.find(LEVEL_PIT_FLOAT) != string::npos || messages.find(LEVEL_PIT_HERE) != string::npos) {
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_PIT);
	} else if (messages.find(LEVEL_POLYMORPH_TRAP_ESCAPE) != string::npos) {
		/* polymorph traps disappear on use */
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_POLYMORPH);
	} else if (messages.find(LEVEL_ROLLING_BOULDER_TRAP_ESCAPE) != string::npos || messages.find(LEVEL_ROLLING_BOULDER_TRAP_HERE) != string::npos || messages.find(LEVEL_ROLLING_BOULDER_TRAP_TRIGGER) != string::npos) {
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_ROLLING_BOULDER);
	} else if (messages.find(LEVEL_RUST_TRAP_ESCAPE) != string::npos || messages.find(LEVEL_RUST_TRAP_HERE) != string::npos || messages.find(LEVEL_RUST_TRAP_TRIGGER) != string::npos) {
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_RUST);
	} else if (messages.find(LEVEL_SLEEPING_GAS_TRAP_ESCAPE) != string::npos || messages.find(LEVEL_SLEEPING_GAS_TRAP_HERE) != string::npos || messages.find(LEVEL_SLEEPING_GAS_TRAP_TRIGGER) != string::npos) {
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_SLEEPING_GAS);
	} else if (messages.find(LEVEL_SPIKED_PIT_ESCAPE) != string::npos || messages.find(LEVEL_SPIKED_PIT_FLOAT) != string::npos || messages.find(LEVEL_SPIKED_PIT_HERE) != string::npos) {
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_SPIKED_PIT);
	} else if (messages.find(LEVEL_SQUEAKY_BOARD_ESCAPE) != string::npos || messages.find(LEVEL_SQUEAKY_BOARD_FLOAT) != string::npos || messages.find(LEVEL_SQUEAKY_BOARD_HERE) != string::npos || messages.find(LEVEL_SQUEAKY_BOARD_TRIGGER) != string::npos) {
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_SQUEAKY_BOARD);
	} else if (messages.find(LEVEL_TELEPORTATION_TRAP_ESCAPE) != string::npos || messages.find(LEVEL_TELEPORTATION_TRAP_HERE) != string::npos) {
		/* no message when triggered (unless telecontrol, but that can trigger on teleportitis too) */
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_TELEPORTATION);
	} else if (messages.find(LEVEL_TRAP_DOOR_ESCAPE) != string::npos || messages.find(LEVEL_TRAP_DOOR_FLOAT) != string::npos) {
		/* can't use messages for falling in, by the time we read the message, we're somewhere else */
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_TRAP_DOOR);
	} else if (messages.find(LEVEL_WEB_ESCAPE) != string::npos || messages.find(LEVEL_WEB_FLOAT) != string::npos || messages.find(LEVEL_WEB_HERE) != string::npos || messages.find(LEVEL_WEB_TRIGGER) != string::npos) {
		setDungeonSymbol(Saiph::position(), TRAP);
		setDungeonSymbolValue(Saiph::position(), TRAP_WEB);
	} else if (messages.find(LEVEL_WALL_UNDIGGABLE) != string::npos) {
		_walls_diggable = false;
	} else if (messages.find(LEVEL_FLOOR_OR_GROUND_UNDIGGABLE) != string::npos) {
		_floor_diggable = false;
	}

	/* attempt to detect minetown */
	if (branch() == BRANCH_MINES && depth() >= 5 && depth() <= 8 && (messages.find(LEVEL_SHOP_ON_LEVEL1) != string::npos || messages.find(LEVEL_SHOP_ON_LEVEL2) != string::npos || messages.find(LEVEL_SHOP_ON_LEVEL3) != string::npos)) {
		/* only place in the mine where we get messages about shops is in minetown */
		branch(BRANCH_MINETOWN);
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
		s->second.symbol(ILLEGAL_ITEM);
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
		s->second.symbol(ILLEGAL_ITEM);
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
		s->second.symbol(ILLEGAL_ITEM);
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
	if (s != _stashes.end()) {
		s->second.items().clear();
		StashChanged sc(Coordinate(Saiph::position().level(), point));
		EventBus::broadcast(&sc);
	}
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

	/* if we're in the mines, at the right depth and drawing an open/closed door,
	 * then we're probably in minetown */
	if (branch() == BRANCH_MINES && depth() >= 5 && depth() <= 8 && (symbol == OPEN_DOOR || symbol == CLOSED_DOOR))
		branch(BRANCH_MINETOWN);
}

void Level::setDungeonSymbolValue(const Point& point, int value) {
	if (!point.insideMap())
		return;
	_symbols[_map[point.row()][point.col()].symbol()][point] = value;
}

bool Level::isCompletelyOpen() const {
	if (symbols(SOLID_ROCK).size() > 0)
		return false;
	if (symbols(TREE).size() > 0)
		return false;
	if (symbols(CLOSED_DOOR).size() > 0)
		return false;
	if (symbols(VERTICAL_WALL).size() > 0)
		return false;
	if (symbols(HORIZONTAL_WALL).size() > 0)
		return false;
	return true;
}

void Level::increaseAdjacentSearchCount(const Point& point, int count) {
	/* increase search count for adjacent points to given point */
	Point p = point;
	for (p.row(point.row() - 1); p.row() <= point.row() + 1; p.moveSouth()) {
		for (p.col(point.col() - 1); p.col() <= point.col() + 1; p.moveEast()) {
			if (!p.insideMap())
				continue;
			_map[p.row()][p.col()].searchInc(count);
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
		if (symbol == FOUNTAIN && _branch == BRANCH_MINETOWN)
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
				bool broadcast = s->second.symbol() != ILLEGAL_ITEM;
				/* top symbol/color changed, update */
				s->second.symbol(symbol);
				s->second.color(color);
				/* broadcast StashChanged unless we looked at the stash last turn */
				if (broadcast) {
					StashChanged sc(Coordinate(Saiph::position().level(), point));
					EventBus::broadcast(&sc);
				}
			}
		} else {
			/* new stash */
			_stashes[point] = Stash(symbol, color);
			/* broadcast StashChanged */
			StashChanged sc(Coordinate(Saiph::position().level(), point));
			EventBus::broadcast(&sc);
		}
	} else if (symbol == t.symbol()) {
		/* if there ever was a stash here, it's gone now */
		_stashes.erase(point);
	}

	/* update monsters */
	if (_monster[symbol] && point != Saiph::position()) {
		_monster_points.insert(point);
	} else {
		_monster_points.erase(point);
	}
}

void Level::setFarlookResults(const map<Point, string>& res) {
	if (_farlooked_turn != World::internalTurn()) {
		_farlooked_turn = World::internalTurn();
		_turn_farlooks.clear();
	}

	for (map<Point, string>::const_iterator i = res.begin(); i != res.end(); ++i)
		_turn_farlooks[i->first] = i->second;
}

bool Level::parseFarlook(Point c, bool& shopkeeper, bool& priest, int& attitude, std::string& name, const data::Monster*& data) {
	map<Point,string>::iterator pfarlook = _turn_farlooks.find(c);
	unsigned char symbol = World::view(c);
	int color = World::color(c);
	if (pfarlook == _turn_farlooks.end())
		return false;

	string& messages = pfarlook->second;
	if (!(messages[2] != ' ' && messages[3] == ' ' && messages[4] == ' ' && messages[5] == ' ')) {
		Debug::warning() << "Bogus farlook result " << messages << endl;
		return false;
	}

	string::size_type pos = string::npos;

	if ((pos = messages.find(" (peaceful ")) != string::npos) {
		/* it's friendly */
		attitude = FRIENDLY;
		pos += sizeof (" (peaceful ") - 1;
	} else if ((pos = messages.find(" (")) != string::npos) {
		/* hostile */
		if (messages.find(" (Oracle", pos) == pos)
			attitude = FRIENDLY; // never attack oracle
		else
			attitude = HOSTILE;
		pos += sizeof (" (") - 1;
	}
	// shopkeepers are always white @, and their names are capitalized
	shopkeeper = (pos != string::npos && pos < messages.size() && symbol == '@' && color == BOLD_WHITE && messages[pos] >= 'A' && messages[pos] <= 'Z');

	priest = (messages.find("priest of ", pos) != string::npos || messages.find("priestess of ", pos) != string::npos);

	string::size_type pos2 = messages.find(" - ", pos);
	if (pos2 == string::npos)
		pos2 = messages.find(")", pos);
	if (pos2 != string::npos) {
		name = messages.substr(pos, pos2 - pos);
		data = data::Monster::monster(name);
	} else
		data = 0;

	return true;
}

vector<Point> Level::farlooksNeeded() {
	vector<Point> ret;

	if (Saiph::hallucinating())
		return ret;

	for (set<Point>::const_iterator look_at = _monster_points.begin(); look_at != _monster_points.end(); ++look_at) {
		unsigned char symbol = World::view(*look_at);
		int color = World::color(*look_at);
		if (symbol == 'I' || (symbol == 'm' && color == BLUE))
			continue; // don't farlook 'I' or 'm' monsters
		map<Point, string>::iterator c = _turn_farlooks.find(*look_at);
		if (c != _turn_farlooks.end() && _farlooked_turn == World::internalTurn())
			continue; // already checked this monster this turn
		ret.push_back(*look_at);
	}

	return ret;
}

void Level::updateMonsters() {
	/* remove monsters that seems to be gone
	 * and make monsters we can't see !visible */

	if (farlooksNeeded().size() > 0)
		return;

	if (World::internalTurn() != _farlooked_turn)
		_turn_farlooks.clear();

	for (set<Point>::iterator pi = _monster_points.begin(); pi != _monster_points.end(); ++pi) {
		Point point = *pi;
		unsigned char symbol = World::view(point);
		int color = World::color(point);

		/* add a monster, or update position of an existing monster */
		unsigned char msymbol;
		if ((color >= INVERSE_BLACK && color <= INVERSE_WHITE) || (color >= INVERSE_BOLD_BLACK && color <= INVERSE_BOLD_WHITE))
			msymbol = PET;
		else
			msymbol = symbol;

		bool shopkeeper = false, priest = false;
		const data::Monster* data = 0;
		int attitude = ATTITUDE_UNKNOWN;
		std::string name;

		parseFarlook(point, shopkeeper, priest, attitude, name, data);

		/* find nearest monster */
		int min_distance = INT_MAX;
		Monster* nearest = 0;
		// we only care about recently seen monsters here
		for (map<int, Monster*>::iterator m = Monster::byLastSeen().lower_bound(World::internalTurn() - MAX_MONSTER_MOVE); m != Monster::byLastSeen().end(); ++m) {
			if (m->second->symbol() != msymbol || m->second->color() != color)
				continue; // not the same monster
			if (m->second->lastSeen() == (int)World::internalTurn())
				continue; // already accounted for
			if (m->second->lastSeenPos().level() != _level)
				continue; // not interested
			/* see if this monster is closer than the last found monster */
			int distance = Point::gridDistance(m->second->lastSeenPos(), point);
			if (distance > MAX_MONSTER_MOVE)
				continue; // too far away from where we last saw it, probably new monster
			else if (distance >= min_distance)
				continue;
			else if ((m->second->priest() || m->second->shopkeeper()) && distance > 1)
				continue; // shopkeepers and priests are very close to eachother in minetown
			/* it is */
			min_distance = distance;
			nearest = m->second;
		}

		if (nearest != 0) {
			if (point != nearest->lastSeenPos()) {
				nearest->lastMoved(World::internalTurn());
				nearest->lastSeenPos(Coordinate(_level, point));
			}
		} else {
			/* add monster */
			string id;
			if (data && (data->genoFlags() & G_UNIQ) != 0) {
				id = name;
			}
			nearest = new Monster(id);
			nearest->lastMoved(World::internalTurn());
			nearest->lastSeenPos(Coordinate(_level, point));
		}
		nearest->symbol(msymbol);
		nearest->color(color);
		nearest->lastSeen(World::internalTurn());
		nearest->attitude(attitude);
		nearest->shopkeeper(shopkeeper);
		nearest->priest(priest);
		nearest->data(data);
	}

	for (map<Point, Monster*>::iterator m = _monsters.begin(); m != _monsters.end(); ++m)
		_map[m->first.row()][m->first.col()].monster(ILLEGAL_MONSTER);
	_monsters.clear();

	/* set monsters on tiles */
	for (map<int, Monster*>::iterator m = Monster::byLastSeen().lower_bound(World::internalTurn() - MAX_MONSTER_MOVE); m != Monster::byLastSeen().end(); ++m) {
		Coordinate c = m->second->lastSeenPos();
		if (c.level() != _level)
			continue; // not interested
		m->second->visible(m->second->lastSeen() == (int)World::internalTurn());
		_map[c.row()][c.col()].monster(m->second->symbol());
		_monsters[c] = m->second;
	}
}

void Level::updatePathMap() {
	Point from = Saiph::position();
	Point to = from;
	_pathing_queue_size = 0;

	/* set node we're standing on */
	_map[from.row()][from.col()].updatePath(NOWHERE, 0, 0);

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
		next.updatePath(direction, distance + 1, cost);
	} else if (cost == next.cost() && cost == UNREACHABLE) {
		next.updatePath(direction, distance + 1, UNPASSABLE);
	}
}
