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
			if (false /* TODO: isRogueLevel */)
				symbol = remapRogueDungeonSymbol(symbol);
			else if (isDungeon(symbol))
				symbol = remapDungeonSymbol(symbol, color);
			/* TODO: update map? */
			if (isItem(symbol)) {
				/* TODO: update item? */
			} else if (isMonster(symbol)) {
				if ((color >= INVERSE_BLACK && color <= INVERSE_WHITE) || (color >= INVERSE_BOLD_BLACK && color <= INVERSE_BOLD_WHITE))
					symbol = PET;
				/* TODO: update monster? */
			}
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
	/* colors & attributes */
	immutable int NO_COLOR             =   0;
	immutable int BOLD                 =   1;
	immutable int INVERSE              =   7;
	immutable int BOLD_OFFSET          =  60;
	immutable int INVERSE_OFFSET       =  10;
	immutable int BLACK                =  30;
	immutable int RED                  =  31;
	immutable int GREEN                =  32;
	immutable int YELLOW               =  33;
	immutable int BLUE                 =  34;
	immutable int MAGENTA              =  35;
	immutable int CYAN                 =  36;
	immutable int WHITE                =  37;
	immutable int INVERSE_BLACK        =  40;
	immutable int INVERSE_RED          =  41;
	immutable int INVERSE_GREEN        =  42;
	immutable int INVERSE_YELLOW       =  43;
	immutable int INVERSE_BLUE         =  44;
	immutable int INVERSE_MAGENTA      =  45;
	immutable int INVERSE_CYAN         =  46;
	immutable int INVERSE_WHITE        =  47;
	immutable int BOLD_BLACK           =  90;
	immutable int BOLD_RED             =  91;
	immutable int BOLD_GREEN           =  92;
	immutable int BOLD_YELLOW          =  93;
	immutable int BOLD_BLUE            =  94;
	immutable int BOLD_MAGENTA         =  95;
	immutable int BOLD_CYAN            =  96;
	immutable int BOLD_WHITE           =  97;
	immutable int INVERSE_BOLD_BLACK   = 100;
	immutable int INVERSE_BOLD_RED     = 101;
	immutable int INVERSE_BOLD_GREEN   = 102;
	immutable int INVERSE_BOLD_YELLOW  = 103;
	immutable int INVERSE_BOLD_BLUE    = 104;
	immutable int INVERSE_BOLD_MAGENTA = 105;
	immutable int INVERSE_BOLD_CYAN    = 106;
	immutable int INVERSE_BOLD_WHITE   = 107;
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
	immutable char MAGIC_PORTAL                       =  140;
	/* other symbols */
	immutable char PET = 141;

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

private:
	/* mapping */
	immutable int IS_MONSTER  = 1 << 0;
	immutable int IS_ITEM     = 1 << 1;
	immutable int IS_DUNGEON  = 1 << 2;
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
		WEAPON   : IS_ITEM,
		ARMOR    : IS_ITEM,
		RING     : IS_ITEM,
		AMULET   : IS_ITEM,
		TOOL     : IS_ITEM,
		FOOD     : IS_ITEM,
		POTION   : IS_ITEM,
		SCROLL   : IS_ITEM,
		SPELLBOOK: IS_ITEM,
		WAND     : IS_ITEM,
		GOLD     : IS_ITEM,
		GEM      : IS_ITEM,
		STATUE   : IS_ITEM,
		BOULDER  : IS_ITEM | IS_DUNGEON, // this one is unique
		// dungeon
		SOLID_ROCK                        : IS_DUNGEON,
		VERTICAL_WALL                     : IS_DUNGEON,
		HORIZONTAL_WALL                   : IS_DUNGEON,
		FLOOR                             : IS_DUNGEON,
		OPEN_DOOR                         : IS_DUNGEON,
		CLOSED_DOOR                       : IS_DUNGEON,
		IRON_BARS                         : IS_DUNGEON,
		TREE                              : IS_DUNGEON,
		CORRIDOR                          : IS_DUNGEON,
		STAIRS_UP                         : IS_DUNGEON,
		STAIRS_DOWN                       : IS_DUNGEON,
		ALTAR                             : IS_DUNGEON,
		GRAVE                             : IS_DUNGEON,
		THRONE                            : IS_DUNGEON,
		SINK                              : IS_DUNGEON,
		FOUNTAIN                          : IS_DUNGEON,
		WATER                             : IS_DUNGEON,
		ICE                               : IS_DUNGEON,
		LAVA                              : IS_DUNGEON,
		LOWERED_DRAWBRIDGE                : IS_DUNGEON,
		RAISED_DRAWBRIDGE                 : IS_DUNGEON,
		TRAP                              : IS_DUNGEON,
		UNKNOWN_TILE                      : IS_DUNGEON,
		UNKNOWN_TILE_DIAGONALLY_UNPASSABLE: IS_DUNGEON,
		UNKNOWN_TILE_UNPASSABLE           : IS_DUNGEON,
		ROGUE_STAIRS                      : IS_DUNGEON,
		MAGIC_PORTAL                      : IS_DUNGEON
	];

	char remapDungeonSymbol(char symbol, int color) {
		switch (symbol) {
			case CORRIDOR:
				return (color == CYAN ? IRON_BARS : (color == GREEN ? TREE : symbol));

			case FLOOR:
				return (color == CYAN ? ICE : (color == YELLOW ? LOWERED_DRAWBRIDGE : symbol));

			case FOUNTAIN:
				return (color == NO_COLOR ? SINK : symbol);

			case GRAVE:
				return (color == YELLOW ? THRONE : symbol);

			case HORIZONTAL_WALL:
			case VERTICAL_WALL:
				return (color == YELLOW ? OPEN_DOOR : symbol);

			case WATER:
				return (color == RED ? LAVA : symbol);

			case TRAP:
				return (color == BOLD_MAGENTA ? MAGIC_PORTAL : symbol);

			default:
				return symbol;
		}
	}

	char remapRogueDungeonSymbol(char symbol) {
		switch (symbol) {
			case SPELLBOOK:
				return OPEN_DOOR;

			case ':':
				return FOOD;

			case ']':
				return ARMOR;

			case GEM:
				return GOLD;

			case ',':
				return AMULET;

			case FOOD:
				return ROGUE_STAIRS;

			default:
				return symbol;
		}
	}
