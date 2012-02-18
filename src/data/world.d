module data.world;

import data.level;
import data.point;
import io.log;
import std.stdio;

class World {
	public:
		static bool question;
		static bool menu;

		static void updateLevel(string name) {
			/* TODO */
		}

		static void updateMapPoint(Point p, char symbol, int color) {
			/* TODO */
		}

	private:
		static Logger log;
		static Level[] levels;
		static Point[] changedMapLocations;

		static this() {
			log = new Logger("world");
		}
}

public:
/* dimensions */
immutable int MAP_ROW_BEGIN =  1;
immutable int MAP_ROW_END   = 21;
immutable int MAP_COL_BEGIN =  0;
immutable int MAP_COL_END   = 79;
/* item symbols */
immutable char WEAPON    = ')';
immutable char ARMOR     = '[';
immutable char RING      = '=';
immutable char AMULET    = '"';
immutable char TOOL      = '(';
immutable char FOOD      = '%';
immutable char POTION    = '!';
immutable char SCROLL    = '?';
immutable char SPELLBOOK = '+';
immutable char WAND      = '/';
immutable char GOLD      = '$';
immutable char GEM       = '*';
immutable char STATUE    = '`';
immutable char BOULDER   = '0';
/* dungeon symbols */
immutable char SOLID_ROCK                         =  ' ';
immutable char VERTICAL_WALL                      =  '|';
immutable char HORIZONTAL_WALL                    =  '-';
immutable char FLOOR                              =  '.';
immutable char OPEN_DOOR                          =  128;
immutable char CLOSED_DOOR                        =  '7';
immutable char IRON_BARS                          =  129;
immutable char TREE                               =  130;
immutable char CORRIDOR                           =  '#';
immutable char STAIRS_UP                          =  '<';
immutable char STAIRS_DOWN                        =  '>';
immutable char ALTAR                              =  '_';
immutable char GRAVE                              = '\\';
immutable char THRONE                             =  131;
immutable char SINK                               =  132;
immutable char FOUNTAIN                           =  '{';
immutable char WATER                              =  '}';
immutable char ICE                                =  133;
immutable char LAVA                               =  134;
immutable char LOWERED_DRAWBRIDGE                 =  135;
immutable char RAISED_DRAWBRIDGE                  =  '8';
immutable char TRAP                               =  '^';
immutable char UNKNOWN_TILE                       =  136;
immutable char UNKNOWN_TILE_DIAGONALLY_UNPASSABLE =  137;
immutable char UNKNOWN_TILE_UNPASSABLE            =  138;
immutable char ROGUE_STAIRS                       =  139;
immutable char MAGIC_PORTAL                       =  141;
/* other symbols */
immutable char PET = 255;
/* mapping */
immutable int IS_MONSTER  = 1 << 0;
immutable int IS_ITEM     = 1 << 1;
immutable int IS_PASSABLE = 1 << 2; // TODO: do we really need this? can't we set a "path cost" to 0 to mark it as unpassable instead?
immutable int IS_DUNGEON  = 1 << 3;
immutable int[] SYMBOLS = [
// monsters
'&' : IS_MONSTER,
'\'': IS_MONSTER,
'1' : IS_MONSTER,
'2' : IS_MONSTER,
'3' : IS_MONSTER,
'4' : IS_MONSTER,
'5' : IS_MONSTER,
':' : IS_MONSTER,
';' : IS_MONSTER,
'@' : IS_MONSTER,
'A' : IS_MONSTER,
'B' : IS_MONSTER,
'C' : IS_MONSTER,
'D' : IS_MONSTER,
'E' : IS_MONSTER,
'F' : IS_MONSTER,
'G' : IS_MONSTER,
'H' : IS_MONSTER,
'I' : IS_MONSTER,
'J' : IS_MONSTER,
'K' : IS_MONSTER,
'L' : IS_MONSTER,
'M' : IS_MONSTER,
'N' : IS_MONSTER,
'O' : IS_MONSTER,
'P' : IS_MONSTER,
'Q' : IS_MONSTER,
'R' : IS_MONSTER,
'S' : IS_MONSTER,
'T' : IS_MONSTER,
'U' : IS_MONSTER,
'V' : IS_MONSTER,
'W' : IS_MONSTER,
'X' : IS_MONSTER,
'Y' : IS_MONSTER,
'Z' : IS_MONSTER,
'a' : IS_MONSTER,
'b' : IS_MONSTER,
'c' : IS_MONSTER,
'd' : IS_MONSTER,
'e' : IS_MONSTER,
'f' : IS_MONSTER,
'g' : IS_MONSTER,
'h' : IS_MONSTER,
'i' : IS_MONSTER,
'j' : IS_MONSTER,
'k' : IS_MONSTER,
'l' : IS_MONSTER,
'm' : IS_MONSTER,
'n' : IS_MONSTER,
'o' : IS_MONSTER,
'p' : IS_MONSTER,
'q' : IS_MONSTER,
'r' : IS_MONSTER,
's' : IS_MONSTER,
't' : IS_MONSTER,
'u' : IS_MONSTER,
'v' : IS_MONSTER,
'w' : IS_MONSTER,
'x' : IS_MONSTER,
'y' : IS_MONSTER,
'z' : IS_MONSTER,
'~' : IS_MONSTER,
PET : IS_MONSTER,
// items
WEAPON   : IS_ITEM | IS_PASSABLE,
ARMOR    : IS_ITEM | IS_PASSABLE,
RING     : IS_ITEM | IS_PASSABLE,
AMULET   : IS_ITEM | IS_PASSABLE,
TOOL     : IS_ITEM | IS_PASSABLE,
FOOD     : IS_ITEM | IS_PASSABLE,
POTION   : IS_ITEM | IS_PASSABLE,
SCROLL   : IS_ITEM | IS_PASSABLE,
SPELLBOOK: IS_ITEM | IS_PASSABLE,
WAND     : IS_ITEM | IS_PASSABLE,
GOLD     : IS_ITEM | IS_PASSABLE,
GEM      : IS_ITEM | IS_PASSABLE,
STATUE   : IS_ITEM | IS_PASSABLE,
BOULDER  : IS_ITEM,
// dungeon
SOLID_ROCK                        : IS_DUNGEON,
VERTICAL_WALL                     : IS_DUNGEON,
HORIZONTAL_WALL                   : IS_DUNGEON,
FLOOR                             : IS_DUNGEON | IS_PASSABLE,
OPEN_DOOR                         : IS_DUNGEON | IS_PASSABLE,
CLOSED_DOOR                       : IS_DUNGEON,
IRON_BARS                         : IS_DUNGEON,
TREE                              : IS_DUNGEON,
CORRIDOR                          : IS_DUNGEON | IS_PASSABLE,
STAIRS_UP                         : IS_DUNGEON | IS_PASSABLE,
STAIRS_DOWN                       : IS_DUNGEON | IS_PASSABLE,
ALTAR                             : IS_DUNGEON | IS_PASSABLE,
GRAVE                             : IS_DUNGEON | IS_PASSABLE,
THRONE                            : IS_DUNGEON | IS_PASSABLE,
SINK                              : IS_DUNGEON | IS_PASSABLE,
FOUNTAIN                          : IS_DUNGEON | IS_PASSABLE,
WATER                             : IS_DUNGEON,
ICE                               : IS_DUNGEON | IS_PASSABLE,
LAVA                              : IS_DUNGEON,
LOWERED_DRAWBRIDGE                : IS_DUNGEON | IS_PASSABLE,
RAISED_DRAWBRIDGE                 : IS_DUNGEON,
TRAP                              : IS_DUNGEON | IS_PASSABLE,
UNKNOWN_TILE                      : IS_DUNGEON | IS_PASSABLE,
UNKNOWN_TILE_DIAGONALLY_UNPASSABLE: IS_DUNGEON,
UNKNOWN_TILE_UNPASSABLE           : IS_DUNGEON,
ROGUE_STAIRS                      : IS_DUNGEON | IS_PASSABLE,
MAGIC_PORTAL                      : IS_DUNGEON | IS_PASSABLE
];

bool insideMap(Point p) {
	return !(p.row < MAP_ROW_BEGIN || p.row > MAP_ROW_END || p.col < MAP_COL_BEGIN || p.col > MAP_COL_END);
}

bool isMonster(char symbol) {
	return (SYMBOLS[symbol] & IS_MONSTER) != 0;
}

bool isItem(char symbol) {
	return (SYMBOLS[symbol] & IS_ITEM) != 0;
}

bool isDungeon(char symbol) {
	return (SYMBOLS[symbol] & IS_DUNGEON) != 0;
}

bool isPassable(char symbol) {
	return (SYMBOLS[symbol] & IS_PASSABLE) != 0;
}
