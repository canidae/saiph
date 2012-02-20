module data.world;

import data.level;
import data.point;
import io.connection;
import logger;
import main;
import std.array;
import std.conv;
import std.stdio;
import std.string;

class World {
	public:
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
		static Level[] levels;
		static Point[] changedMapLocations;
		static bool _question;
		static bool _menu;
		static string[] _messages;

		static void newFrame() {
			_question = false;
			_menu = false;
			_messages.length = 0;
			changedMapLocations.length = 0;
		}

		static void updateLevel(string name) {
			/* TODO */
		}

		static void updateMapPoint(Point p, char symbol, int color) {
			/* remap ambiguous symbols */
			if (false /* TODO: isRogueLevel */)
				symbol = remapRogueDungeonSymbol(symbol);
			else if (isDungeon(symbol))
				symbol = remapDungeonSymbol(symbol, color);
			/* update map */
			/* TODO */
			/* update items & monsters */
			if (isItem(symbol)) {
				/* TODO */
			} else if (isMonster(symbol)) {
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
}

public:
	/* dimensions */
	immutable int ROWS = 24;
	immutable int COLS = 80;
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

	/* this should only be called by main() */
	bool update(Connection conn) {
		World.newFrame();
		changedScreenLocations.length = 0;
		++sequence;
		return parse(conn);
	}

private:
	/* stuff we look for in data from game */
	immutable string MORE = "--More--";
	immutable string END = "(end) ";
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

	struct PointData {
		int sequence;
		char symbol;
		int color;
	}

	Logger log; /* TODO: ask why you can't: = new Logger("world"); */
	Point cursor;
	Point[] changedScreenLocations;
	PointData[COLS][ROWS] screen;
	int sequence;
	int color;
	bool bold;
	bool inverse;

	/* static constructor (see TODO above) */
	static this() {
		log = new Logger("world");
	}

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

	void updateScreen(Point p, char symbol, int color) {
		if (screen[p.row][p.col].sequence != sequence)
			changedScreenLocations ~= p;
		screen[p.row][p.col] = PointData(sequence, symbol, color);
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
