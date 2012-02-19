module io.connection;

import data.point;
import data.world;
import io.log;
import std.conv;
import std.stdio;
import std.string;

abstract class Connection {
	public:
		abstract void connect();

		abstract void disconnect();

		abstract void send(char[] data);

		bool update() {
			World.newFrame();
			changedScreenLocations.length = 0;
			++sequence;
			return parse(receive());
		}

	protected:
		abstract char[] receive();

	private:
		struct PointData {
			int sequence;
			char symbol;
			int color;
		}

		static Logger log; /* TODO: ask why you can't: = new Logger("connection"); */
		Point cursor;
		Point[] changedScreenLocations;
		PointData[COLS][ROWS] screen;
		int sequence;
		int color;
		bool bold;
		bool inverse;

		/* static constructor (see TODO above) */
		static this() {
			log = new Logger("connection");
		}

		void updateScreen(Point p, char symbol, int color) {
			if (screen[p.row][p.col].sequence != sequence)
				changedScreenLocations ~= p;
			screen[p.row][p.col] = PointData(sequence, symbol, color);
		}

		bool parse(char[] data) {
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
					write(screen[r][c]);
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
				send([' ']);
				parse(receive());
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
}

public:
/* dimensions */
immutable int ROWS = 24;
immutable int COLS = 80;
/* stuff we look for in data */
immutable string MORE = "--More--";
immutable string END = "(end) ";
