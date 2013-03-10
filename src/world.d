module world;

import std.array;
import std.conv;
import std.stdio;
import std.string;

import ai.ai;
import action;
import connection;
import logger;

struct Point {
	int row;
	int col;

	Point moveNorthwest() {
		--row;
		--col;
		return this;
	}

	Point moveNorth() {
		--row;
		return this;
	}

	Point moveNortheast() {
		--row;
		++col;
		return this;
	}

	Point moveEast() {
		++col;
		return this;
	}

	Point moveSoutheast() {
		++row;
		++col;
		return this;
	}

	Point moveSouth() {
		++row;
		return this;
	}

	Point moveSouthwest() {
		++row;
		--col;
		return this;
	}

	Point moveWest() {
		--col;
		return this;
	}

	bool opEquals()(Point p) const {
		return row == p.row && col == p.col;
	}

	int opCmp()(Point p) const {
		if (row < p.row || (row == p.row && col < p.col))
			return -1;
		if (row > p.row || (row == p.row && col > p.col))
			return 1;
		return 0;
	}
}

struct Coordinate {
	Level level;
	Point point;

	bool opEquals()(Coordinate c) const {
		return level == c.level && point == c.point;
	}

	int opCmp()(Coordinate c) const {
		if (level < c.level || (level == c.level && point < c.point))
			return -1;
		if (level > c.level || (level == c.level && point > c.point))
			return 1;
		return 0;
	}

	alias point this;
}

struct Tile {
	Coordinate coordinate;
	char symbol;
	bool shop;

	this(Coordinate coordinate) {
		this.coordinate = coordinate;
	}

	bool opEquals()(Tile t) const {
		return coordinate == t.coordinate;
	}

	int opCmp()(Tile t) const {
		return coordinate.opCmp(t.coordinate);
	}

	alias coordinate this;
}

struct PathNode {
	char direction;
	uint distance;
	uint cost;
}

class Level {
	@property string name() {
		return _name;
	}

	@property int depth() {
		return _depth;
	}

	@property int branch() {
		return _branch;
	}

	@property bool wallsDiggable() {
		return _wallsDiggable;
	}

	@property bool floorDiggable() {
		return _floorDiggable;
	}

	Tile tileAt(Point p) {
		return _tiles[p.row][p.col];
	}

	bool opEquals()(Level l) {
		return id == l.id;
	}

	int opCmp()(Level l) {
		return id - l.id;
	}

private:
	Tile[MAP_COL_END + 1][MAP_ROW_END + 1] _tiles;
	PathNode[MAP_COL_END + 1][MAP_ROW_END + 1] _pathMap;
	Point[PATHING_QUEUE_SIZE] _pathMapQueue;
	int _id;
	string _name;
	int _depth;
	int _branch;
	bool _wallsDiggable;
	bool _floorDiggable;
	int _pathMapQueueMaxIndex;

	this(int id, string name, int depth) {
		this._id = id;
		this._name = name;
		this._depth = depth;
		foreach (int r; MAP_ROW_BEGIN .. MAP_ROW_END + 1) {
			foreach (int c; MAP_COL_BEGIN .. MAP_COL_END + 1)
				_tiles[r][c] = Tile(Coordinate(this, Point(r, c)));
		}
	}

	void updatePathMap(Point from) {
		/* clear path map */
		foreach (int r; MAP_ROW_BEGIN .. MAP_ROW_END + 1)
			_pathMap[r][] = PathNode(NOWHERE, UNREACHABLE, UNREACHABLE);
		/* distance & cost to tile we're standing on are both 0 */
		_pathMap[from.row][from.col] = PathNode(NOWHERE, 0, 0);
		/* then set direction, distance & cost to tiles adjacent to where we're standing */
		_pathMapQueueMaxIndex = -1;
		Point to = from;
		setPathNode(from, to.moveNorth(), NORTH);
		setPathNode(from, to.moveEast(), NORTHEAST);
		setPathNode(from, to.moveSouth(), EAST);
		setPathNode(from, to.moveSouth(), SOUTHEAST);
		setPathNode(from, to.moveWest(), SOUTH);
		setPathNode(from, to.moveWest(), SOUTHWEST);
		setPathNode(from, to.moveNorth(), WEST);
		setPathNode(from, to.moveNorth(), NORTHWEST);
		/* then set direction, distance & cost to the rest of the tiles */
		int index = -1;
		while (index < _pathMapQueueMaxIndex) {
			from = _pathMapQueue[++index];
			to = from;
			setPathNode(from, to.moveNorth(), _pathMap[from.row][from.col].direction);
			setPathNode(from, to.moveEast(), _pathMap[from.row][from.col].direction);
			setPathNode(from, to.moveSouth(), _pathMap[from.row][from.col].direction);
			setPathNode(from, to.moveSouth(), _pathMap[from.row][from.col].direction);
			setPathNode(from, to.moveWest(), _pathMap[from.row][from.col].direction);
			setPathNode(from, to.moveWest(), _pathMap[from.row][from.col].direction);
			setPathNode(from, to.moveNorth(), _pathMap[from.row][from.col].direction);
			setPathNode(from, to.moveNorth(), _pathMap[from.row][from.col].direction);
		}
	}

	void setPathNode(Point from, Point to, char direction) {
		if (!World.insideMap(to))
			return; // target is outside map
		if (_pathMap[from.row][from.col].cost >= _pathMap[to.row][to.col].cost)
			return; // already found a less costly path to this node
		uint cost = calculateMoveCost(from, to);
		if (cost < _pathMap[to.row][to.col].cost) {
			_pathMap[to.row][to.col] = PathNode(direction, _pathMap[from.row][from.col].distance + 1, cost);
			_pathMapQueue[++_pathMapQueueMaxIndex] = to;
		}
	}

	uint calculateMoveCost(Point from, Point to) {
		char toSymbol = _tiles[to.row][to.col].symbol;
		if (!isPassable(toSymbol))
			return UNREACHABLE;
		bool cardinal_move = (to.row == from.row || to.col == from.col);
		if (!cardinal_move) {
			/* diagonal move, some rules apply */
			char fromSymbol = _tiles[from.row][from.col].symbol;
			if (toSymbol == OPEN_DOOR || fromSymbol == OPEN_DOOR)
				return UNREACHABLE; // can't move diagonally into a door
			else if (toSymbol == UNKNOWN_TILE_DIAGONALLY_UNPASSABLE || fromSymbol == UNKNOWN_TILE_DIAGONALLY_UNPASSABLE)
				return UNREACHABLE; // don't know what tile this is, but we know we can't go to/from it diagonally
			else if (!isPassable(_tiles[to.row][from.col].symbol, true /* TODO: !inSokoban() */ ) && !isPassable(_tiles[from.row][to.col].symbol, true /* TODO: !inSokoban() */))
				return UNREACHABLE; // don't allow passing through small cracks, nor move diagonally past boulders in sokoban
			/* TODO:
			else if (polymorphedToGridBug)
				return UNREACHABLE;
			*/
		}
		/* TODO:
		if (toSymbol == TRAP && inSokoban())
			return UNREACHABLE;
		*/
		/* TODO:
		if (_tiles[to.row][to.col].monster !is null && _tiles[to.row][to.col].monster !is pet)
			return UNREACHABLE;
		*/
		uint cost = _pathMap[from.row][from.col].cost + (cardinal_move ? 2 : 3);
		final switch (toSymbol) {
			case FOUNTAIN:
				cost += 4; // better not fight on fountains
				break;

			case GRAVE:
				cost += 4; // better not fight on graves
				break;

			case ALTAR:
				cost += 4; // better not fight on altars
				break;

			case ICE:
				cost += 8; // slippery and risky, try to find a way around (don't try very hard, though)
				break;

			case TRAP:
				cost += 64; // avoid traps
				break;

			case WATER:
				cost += 256; // avoid water
				break;

			case LAVA:
				cost += 512; // lava, hot!
				break;
		}
		return cost;
	}

	bool isPassable(char symbol, bool allowPassingBoulder = false) {
		final switch (symbol) {
			case BOULDER:
				return allowPassingBoulder;

			case WATER:
			case LAVA:
				/* water & lava may be passed when we're levitating or got (fireproof) waterwalking boots */
				// TODO

			case SOLID_ROCK:
			case VERTICAL_WALL:
			case HORIZONTAL_WALL:
			case CLOSED_DOOR:
			case IRON_BARS:
			case TREE:
			case RAISED_DRAWBRIDGE:
			case UNKNOWN_TILE_UNPASSABLE:
				/* these are passable if we're polymorphed into a xorn */
				// TODO
				return false;
		}
		return true;
	}
}

class Saiph {
	/* attributes */
	static int alignment;
	static int charisma;
	static int constitution;
	static int dexterity;
	static int intelligence;
	static int strength;
	static int wisdom;
	/* status */
	static int armor;
	static int encumbrance;
	static int experience;
	static int hunger;
	static int hitpoints;
	static int hitpointsMax;
	static int power;
	static int powerMax;
	/* effects */
	static bool blind;
	static bool confused;
	static bool poisoned;
	static bool hallucinating;
	static bool ill;
	static bool slimed;
	static bool stoned;
	static bool stunned;
	static bool hurtLeg;
	static bool polymorphed;
	static bool engulfed;
	static bool inPit;
	/* position */
	static Coordinate position;
	/* zorkmids */
	static int zorkmids;
}

class World {
	@property static bool question() {
		return _question;
	}

	@property static bool menu() {
		return _menu;
	}

	@property static string[] messages() {
		return _messages;
	}

private:
	static Level[] _levels;
	static Point[] _changedMapLocations;
	static bool _question;
	static bool _menu;
	static string[] _messages;
	static int _levelId;

	static void newFrame() {
		_question = false;
		_menu = false;
		_messages.length = 0;
		_changedMapLocations.length = 0;
	}

	static void updateLevel() {
		/* TODO */
	}

	static void updateMapPoint(Point p, char symbol, int color) {
		if (!insideMap(p))
			return; // outside of map
		/* remap ambiguous symbols */
		if (false /* TODO: isRogueLevel */)
			symbol = remapRogueDungeonSymbol(symbol);
		else if ((SYMBOLS[symbol] & IS_DUNGEON) != 0)
			symbol = remapDungeonSymbol(symbol, color);
		if ((SYMBOLS[symbol] & IS_DUNGEON) != 0) {
			/* update dungeon */
			/* TODO */
		}
		/* no "else if" here, because boulders are considered as both dungeon and item symbol */
		if ((SYMBOLS[symbol] & IS_ITEM) != 0) {
			/* update items */
			/* TODO */
		} else if ((SYMBOLS[symbol] & IS_MONSTER) != 0) {
			/* update monsters */
			if ((color >= INVERSE_BLACK && color <= INVERSE_WHITE) || (color >= INVERSE_BOLD_BLACK && color <= INVERSE_BOLD_WHITE))
				symbol = PET;
			/* TODO */
		}
	}

	static void addMessages(string message) {
		message = strip(message);
		if (message == "")
			return;
		string[] splitted = split(message, "  ");
		foreach (string s; splitted) {
			s = strip(s);
			if (s != "") {
				writefln("Adding message: %s", s);
				_messages ~= s;
			}
		}
	}

	static bool insideMap(Point p) {
		return p.row >= MAP_ROW_BEGIN && p.row <= MAP_ROW_END && p.col >= MAP_COL_BEGIN && p.col <= MAP_COL_END;
	}

	static char remapDungeonSymbol(char symbol, int color) {
		final switch (symbol) {
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
		}
		return symbol;
	}

	static char remapRogueDungeonSymbol(char symbol) {
		final switch (symbol) {
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
		}
		return symbol;
	}
}

private:
/* dimensions */
immutable uint ROWS               = 24;
immutable uint COLS               = 80;
immutable uint MAP_ROW_BEGIN      =  1;
immutable uint MAP_ROW_END        = 21;
immutable uint MAP_COL_BEGIN      =  0;
immutable uint MAP_COL_END        = 79;
immutable uint PATHING_QUEUE_SIZE = 32768; // should be large enough to prevent out-of-bounds when pathing
immutable uint UNREACHABLE        = uint.max;
immutable uint UNPASSABLE         = UNREACHABLE - 1;
/* directions */
immutable char NORTH        =  'k';
immutable char NORTHEAST    =  'u';
immutable char EAST         =  'l';
immutable char SOUTHEAST    =  'n';
immutable char SOUTH        =  'j';
immutable char SOUTHWEST    =  'b';
immutable char WEST         =  'h';
immutable char NORTHWEST    =  'y';
immutable char UP           =  '<';
immutable char DOWN         =  '>';
immutable char NOWHERE      =  '.';
immutable char NO_DIRECTION = '\0';
/* colors & attributes */
immutable uint NO_COLOR             =   0;
immutable uint BOLD                 =   1;
immutable uint INVERSE              =   7;
immutable uint BOLD_OFFSET          =  60;
immutable uint INVERSE_OFFSET       =  10;
immutable uint BLACK                =  30;
immutable uint RED                  =  31;
immutable uint GREEN                =  32;
immutable uint YELLOW               =  33;
immutable uint BLUE                 =  34;
immutable uint MAGENTA              =  35;
immutable uint CYAN                 =  36;
immutable uint WHITE                =  37;
immutable uint INVERSE_BLACK        =  40;
immutable uint INVERSE_RED          =  41;
immutable uint INVERSE_GREEN        =  42;
immutable uint INVERSE_YELLOW       =  43;
immutable uint INVERSE_BLUE         =  44;
immutable uint INVERSE_MAGENTA      =  45;
immutable uint INVERSE_CYAN         =  46;
immutable uint INVERSE_WHITE        =  47;
immutable uint BOLD_BLACK           =  90;
immutable uint BOLD_RED             =  91;
immutable uint BOLD_GREEN           =  92;
immutable uint BOLD_YELLOW          =  93;
immutable uint BOLD_BLUE            =  94;
immutable uint BOLD_MAGENTA         =  95;
immutable uint BOLD_CYAN            =  96;
immutable uint BOLD_WHITE           =  97;
immutable uint INVERSE_BOLD_BLACK   = 100;
immutable uint INVERSE_BOLD_RED     = 101;
immutable uint INVERSE_BOLD_GREEN   = 102;
immutable uint INVERSE_BOLD_YELLOW  = 103;
immutable uint INVERSE_BOLD_BLUE    = 104;
immutable uint INVERSE_BOLD_MAGENTA = 105;
immutable uint INVERSE_BOLD_CYAN    = 106;
immutable uint INVERSE_BOLD_WHITE   = 107;
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
/* stuff we look for in data from game */
immutable string MORE = "--More--";
immutable string END  = "(end) ";
/* mapping */
immutable uint IS_MONSTER  = 1 << 0;
immutable uint IS_ITEM     = 1 << 1;
immutable uint IS_DUNGEON  = 1 << 2;
immutable uint[] SYMBOLS = [
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
	BOULDER  : IS_ITEM | IS_DUNGEON, // this one is unique, being both an item and a dungeon feature
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

struct PointData {
	uint sequence;
	char symbol;
	int color;
}

Logger log; /* TODO: ask why you can't: = new Logger("world"); */
Point cursor;
Point[ROWS * COLS] changedScreenLocations;
PointData[COLS][ROWS] screen;
uint sequence;
uint changedScreenLocationsMaxIndex;
int color;
bool bold;
bool inverse;

/* static constructor (see TODO above) */
static this() {
	log = new Logger("world");
}

bool update(Connection conn) {
	World.newFrame();
	changedScreenLocationsMaxIndex = -1;
	++sequence;
	if (parse(conn)) {
		/* TODO: parse status lines */
		World.updateLevel();
		for (int index = 0; index <= changedScreenLocationsMaxIndex; ++index) {
			Point p = changedScreenLocations[index];
			World.updateMapPoint(p, screen[p.row][p.col].symbol, screen[p.row][p.col].color);
		}
		return true;
	}
	return false;
}

bool parse(Connection conn) {
	char[] data = conn.receive();
	for (int i = 0; i < data.length; ++i) {
		char c = data[i];
		switch (c) {
			case 0, 14, 15:
				/* null characters and invoking character set commands we can ignore */
				break;

			case 8: 
				/* backspace, move cursor left (west) */
				cursor.moveWest();
				break;

			case 10:
				/* linefeed, go down (south) */
				cursor.moveSouth();
				break;

			case 13:
				/* carriage return */
				cursor.col = 0;
				break;

			case 27:
				/* escape sequence */
				c = data[++i];
				switch (c) {
					case 27, '=', '>':
						/* we can ignore <ESC> followed by <ESC>, '=' or '>' */
						break;

					case '(', ')', '*', '+':
						/* designate character set, ignore this and the following character */
						++i;
						break;

					case 'M':
						/* reverse linefeed, move up (north) */
						cursor.moveNorth();
						break;

					case '[':
						/* escape code, handle it */
						int length = parseEscapeSequence(data[i .. $]);
						if (length == -1)
							return false;
						i += length;
						break;

					default:
						/* this is bad */
						log.error("Unexpected escape sequence character: '%s' (%s). ±9 characters: %s", c, cast(int) c, data[(i > 9 ? i - 9 : 0) .. (i + 9 < $ ? 9 : $)]);
						return false;
				}
				break;

			default:
				/* add character to map and move east */
				updateScreen(cursor, c, color);
				cursor.moveEast();
				break;

		}
	}
	/* XXX: just dumping screen for debugging purpopses */
	for (int r = 0; r < screen.length; ++r) {
		for (int c = 0; c < screen[r].length; ++c)
			write(screen[r][c].symbol);
		writeln();
	}
	if (data[$ - MORE.length .. $] == MORE) {
		/* we got more messages waiting, parse current messages and fetch the next messages */
		if (cursor.row == 0) {
			/* only one line with messages, remove "--More--" and add messages */
			string message;
			for (int c = 0; c < screen[0].length; ++c)
				message ~= screen[0][c].symbol;
			World.addMessages(strip(message)[0 .. $ - MORE.length]);
		} else {
			/* multiple lines with messages, may be a list */
			int r = cursor.row;
			int c = cursor.col - cast(int) MORE.length;
			if (c == 0 || screen[r][c - 1].symbol == ' ') {
				/* not a list, just a wrapping long line */
				/* TODO */
			} else {
				/* appears to be a list */
				/* TODO */
			}
		}
		/* then send space and parse the remaining messages */
		conn.send([' ']);
		parse(conn);
	}
	if (cursor.row == 0) {
		/* cursor should only stop on row 0 when we got a question */
		//World.question = true;
		//World.menu = false; // won't have a menu when we got a question, making sure it's false
	}
	/* TODO: if we get a menu, we need to do something clever */
	if (!World.menu) {
		/* no menu and no "--More--", may be a message at first line */
		string message;
		for (int c = 0; c < screen[0].length; ++c)
			message ~= screen[0][c].symbol;
		World.addMessages(message);
	}
	return true;
}

int parseEscapeSequence(char[] data) {
	int divider = -1;
	/* note that we start at data[1], not data[0]. data[0] usually is '[' (with some few exceptions for 'J' and 'K' commands) */
	for (int i = 1; i < data.length; ++i) {
		char c = data[i];
		if (c == 'h' || c == 'l' || c == 'r' || c == 'z') {
			/* these escape sequences can be safely ignored (probably) */
			return i;
		} else if (c == ';') {
			/* divider for values */
			divider = i;
			/* no return here, we're not done with the sequence */
		} else if (c == 'A') {
			/* move cursor up (north) */
			cursor.moveNorth();
			return i;
		} else if (c == 'B') {
			/* move cursor down (south) */
			cursor.moveSouth();
			return i;
		} else if (c == 'C') {
			/* move cursor right (east) */
			cursor.moveEast();
			return i;
		} else if (c == 'D') {
			/* move cursor left (west) */
			cursor.moveWest();
			return i;
		} else if (c == 'H') {
			/* set cursor position */
			if (divider < 0) {
				/* "<ESC>[H" - set cursor to top left corner */
				cursor.row = 0;
				cursor.col = 0;
			} else {
				/* subtract 1 since terminal starts counting at 1 */
				cursor.row = to!int(data[1 .. divider]) - 1;
				cursor.col = to!int(data[divider + 1 .. i]) - 1;
			}
			return i;
		} else if (c == 'J') {
			/* erase in display */
			if (data[i - 1] == '[') {
				/* erase everything below current position */
				for (int row = cursor.row + 1; row < ROWS; ++row) {
					for (int col = 0; col < COLS; ++col)
						updateScreen(Point(row, col), ' ', 0);
				}
			} else if (data[i - 1] == '1') {
				/* erase everything above current position */
				for (int row = cursor.row - 1; row >= 0; --row) {
					for (int col = 0; col < COLS; ++col)
						updateScreen(Point(row, col), ' ', 0);
				}
			} else if (data[i - 1] == '2') {
				/* erase everything */
				for (int row = 0; row < ROWS; ++row) {
					for (int col = 0; col < COLS; ++col)
						updateScreen(Point(row, col), ' ', 0);
				}
			}
			return i;
		} else if (c == 'K') {
			/* erase in line */
			if (data[i - 1] == '[') {
				/* erase everything to the right */
				for (int col = cursor.col; col < COLS; ++col)
					updateScreen(Point(cursor.row, col), ' ', 0);
			} else if (data[i - 1] == '1') {
				/* erase everything to the left */
				for (int col = 0; col < cursor.col; ++col)
					updateScreen(Point(cursor.row, col), ' ', 0);
			} else if (data[i - 1] == '2') {
				/* erase entire line */
				for (int col = 0; col < COLS; ++col)
					updateScreen(Point(cursor.row, col), ' ', 0);
			}
			return i;
		} else if (c == 'm') {
			/* character set (bold, inverse, color, etc) */
			color = 0;
			if (i == 0)
				break;
			int value = to!int(data[1 .. i]);
			switch (value) {
				case NO_COLOR:
					bold = false;
					inverse = false;
					break;

				case BOLD:
					bold = true;
					break;

				case INVERSE:
					inverse = true;
					break;

				default:
					if (bold)
						value += BOLD_OFFSET;
					if (inverse)
						value += INVERSE_OFFSET;
					color = value;
					break;
			}
			return i;
		} else if (c == 27) {
			/* unexpected escape character, hmm */
			log.error("Unexpected <ESC> character in escape sequence: %s", data[0 .. i]);
			return -1;
		} else if (i > 7) {
			/* suspiciously long sequence, hmm */
			log.error("Suspiciously long escape sequence: %s", data[0 .. i]);
			return -1;
		}
	}
	/* shouldn't really end up here */
	return -1;
}

void updateScreen(Point p, char symbol, int color) {
	if (screen[p.row][p.col].sequence != sequence)
		changedScreenLocations[++changedScreenLocationsMaxIndex] = p;
	screen[p.row][p.col] = PointData(sequence, symbol, color);
}

/* main */
void main() {
	Connection conn = new Telnet();
	conn.connect();
	Ai ai = Ai.createAi();
	Action action;
	/* loop here */
	bool active = true;
	while (active) {
		active = update(conn);
		/* update existing action (if any) */
		if (action !is null && action.command != "")
			action.update();
		/* find new action unless we're still executing an action */
		if (action is null || action.command == "") {
			if (World.question) {
				action = ai.handleQuestion();
			} else if (World.menu) {
				action = ai.handleMenu();
			} else {
				ai.parseFrame();
				ai.preAnalyze();
				/* TODO: update path map */
				ai.analyze();
			}
		}
		/* send command (or quit if no command) */
		conn.send(cast(char[]) (action !is null && action.command != "" ? action.command : "#quit\ny q"));
	}
	conn.disconnect();
}
