#include "World.h"

/* constructors */
World::World(Connection *connection) {
	this->connection = connection;
	row = 0;
	col = 0;
	data = new char[BUFFER_SIZE];
	memset(data, '\0', BUFFER_SIZE);
	data_size = -1;
	messages_pos = 0;
	/* fetch the first "frame" */
	update();
}

/* destructors */
World::~World() {
	delete [] data;
}

/* methods */
void World::command(const char *command) {
	/* send a command to nethack */
	connection->send(command);
	update();
}

/* private methods */
void World::handleEscapeSequence(int &pos, int &colour) {
	if (data[pos] == 27) {
		/* sometimes we get 2 escape chars in a row,
		 * just return in those cases */
		return;
	} else if (data[pos] == '[') {
		int divider = -1;
		int start = pos;
		for (; pos < data_size; ++pos) {
			if (data[pos] == ';') {
				/* divider for values */
				divider = pos;
			} else if (data[pos] == 'A') {
				/* move cursor up */
				if (row > 0)
					--row;
				break;
			} else if (data[pos] == 'B') {
				/* move cursor down */
				if (row < ROWS)
					++row;
				break;
			} else if (data[pos] == 'C') {
				/* move cursor right */
				if (col < COLS)
					++col;
				break;
			} else if (data[pos] == 'D') {
				/* move cursor left */
				if (col > 0)
					--col;
				break;
			} else if (data[pos] == 'H') {
				/* set cursor position */
				row = 0;
				col = 0;
				if (divider < 0)
					break;
				/* we got a position */
				int matched = sscanf(&data[start + 1], "%d;%d", &row, &col);
				--row; // terminal starts counting from 1
				--col; // ditto ^^
				if (matched < 2) {
					cerr << "Unable to place cursor" << endl;
					cerr << &data[start] << endl;
					exit(13);
				}
				break;
			} else if (data[pos] == 'J') {
				/* erase in display */
				if (data[pos - 1] == '[') {
					/* erase everything below current position */
					for (int r = row + 1; r < ROWS; ++r) {
						for (int c = 0; c < COLS; ++c)
							map[r][c] = ' ';
					}
				} else if (data[pos - 1] == '1') {
					/* erase everything aboce current position */
					for (int r = row - 1; r >= 0; --r) {
						for (int c = 0; c < COLS; ++c)
							map[r][c] = ' ';
					}
				} else if (data[pos - 1] == '2') {
					/* erase entire display */
					for (int r = 0; r < ROWS; ++r) {
						for (int c = 0; c < COLS; ++c)
							map[r][c] = ' ';
					}
					row = 0;
					col = 0;
					colour = 0;
				} else {
					cerr << "Unhandled sequence: " << endl;
					cerr << &data[pos] << endl;
					exit(9);
				}
				break;
			} else if (data[pos] == 'K') {
				/* erase in line */
				if (data[pos - 1] == '[') {
					/* erase everything to the right */
					for (int c = col; c < COLS; ++c)
						map[row][c] = ' ';
				} else if (data[pos - 1] == '1') {
					/* erase everything to the left */
					for (int c = 0; c < col; ++c)
						map[row][c] = ' ';
				} else if (data[pos - 1] == '2') {
					/* erase entire line */
					for (int c = 0; c < COLS; ++c)
						map[row][c] = ' ';
				} else {
					cerr << "Unhandled sequence: " << endl;
					cerr << &data[pos] << endl;
					exit(9);
				}
				break;
			} else if (data[pos] == 'h') {
				/* can possibly be ignored */
				/* probably [?1049h */
				break;
			} else if (data[pos] == 'l') {
				/* DEC Private Mode Reset? :s */
				break;
			} else if (data[pos] == 'm') {
				/* character attribute (bold, inverted, colour, etc) */
				if (divider > 0) {
					cerr << "Unsupported character colour" << endl;
					cerr << &data[pos] << endl;
					exit(15);
					break;
				}
				colour = 0;
				if (pos == start + 1)
					break;
				int value = 0;
				int matched = sscanf(&data[start + 1], "%d", &value);
				if (matched < 1) {
					cerr << "Expected numeric value for character attribute" << endl;
					cerr << &data[pos] << endl;
					exit(14);
				}
				colour = value;
				break;
			} else if (data[pos] == 'r') {
				/* this is some scrolling crap, ignore it */
				break;
			} else if (data[pos] == 27) {
				/* escape char found, that shouldn't happen */
				cerr << "Escape character found in sequence: " << endl;
				for (int a = start; a <= pos; ++a)
					cerr << (int) data[a] << " - ";
				cerr << endl;
				cerr << &data[start] << endl;
				cerr << data << endl;
				exit(7);
			} else if (pos - start > 7) {
				/* too long escape sequence? */
				cerr << "Suspiciously long sequence: " << endl;
				cerr << &data[pos] << endl;
				exit(8);
			}
		}
		if (pos >= data_size) {
			cerr << "Did not find stop char for sequence" << endl;
			cerr << data << endl;
			exit(6);
		}
	} else if (data[pos] == '(') {
		/* designate character set, ignore */
		++pos;
	} else if (data[pos] == ')') {
		/* designate character set, ignore */
		++pos;
	} else if (data[pos] == '*') {
		/* designate character set, ignore */
		++pos;
	} else if (data[pos] == '+') {
		/* designate character set, ignore */
		++pos;
	} else if (data[pos] == 'M') {
		/* reverse linefeed? */
		if (row > 0)
			--row;
	} else if (data[pos] == '=') {
		/* application numpad?
		 * ignore */
	} else if (data[pos] == '>') {
		/* normal numpad?
		 * ignore */
	} else {
		cerr << "Unsupported escape sequence code at char " << pos << ": ";
		cerr << &data[pos] << endl;
		exit(5);
	}
}

void World::fetchMessages() {
	/* fetch the messages currently displayed in map */
	/* if there's a space before "--More--" we got a list */
	/* this code is ugly :( */
	bool islist = false;
	for (int r = 0; r < ROWS; ++r) {
		string str(map[r]);
		string::size_type pos = str.find(MORE, 0);
		if (pos == string::npos)
			continue;
		/* "--More--" found */
		if (pos == 0 || map[r][pos - 1] == ' ')
			islist = true;
		if (islist) {
			/* add from map[0][pos - 1] to map[r - 1][80] */
			for (int r2 = 0; r2 < r; ++r2)
				fetchMessagesHelper(r2, pos - 1);
		} else {
			/* add from map[0][0] to map[r][80] */
			for (int r2 = 0; r2 <= r; ++r2)
				fetchMessagesHelper(r2, 0);
		}
		/* request next "page" */
		command(" ");
		return;
	}
	if (!islist)
		fetchMessagesHelper(0, 0);
	messages[messages_pos] = '\0';
	messages_pos = 0;
}

void World::fetchMessagesHelper(int row, int startcol) {
	/* copy message into messages array, removing "--More--"
	 * and keeping 2 spaces between messages.
	 * player may encounter messages with 2 spaces in it,
	 * eg. engravings. usually that is quoted. */
	if (row < 0 || row > ROWS || startcol < 0)
		return;
	string str(&map[row][startcol]);
	string::size_type pos = str.find(MORE, 0);
	if (pos != string::npos) {
		/* "--More--" found, remove it */
		str = str.substr(0, pos);
	}
	/* trim */
	string::size_type ltpos = str.find_first_not_of(" ");
	if (ltpos == string::npos)
		return; // no messages
	str = str.substr(ltpos, str.find_last_not_of(" ") + 1);
	/* add two spaces at end */
	str.append(2, ' ');
	/* copy into messages */
	str.copy(&messages[messages_pos], str.length());
	messages_pos += str.length();
}

void World::update() {
	/* update the map */
	int colour = 0; // colour of the char
	data_size = connection->retrieve(data, BUFFER_SIZE);
	cerr << data << endl;
	for (int pos = 0; pos < data_size; ++pos) {
		switch (data[pos]) {
			case 0:
				/* sometimes we get lots of \0 characters.
				 * seemingly this happens when certain effects happen.
				 * for example, gas spore explotion, dagger thrown, etc.
				 * let's simply ignore these */
				break;
			case 8:
				/* backspace.
				 * make it go 1 char left */
				if (col > 0)
					--col;
				break;

			case 10:
				/* various control characters we'll ignore */
				break;

			case 13:
				/* carriage return.
				 * make it go to beginning of line */
				col = 0;
				break;

			case 14:
				/* shift out, invoke G1 character set */
				break;

			case 15:
				/* shift in, invoke G0 character set */
				break;

			case 27:
				/* escape sequence coming up */
				handleEscapeSequence(++pos, colour);
				break;

			default:
				/* add this char to the map */
				if (col >= COLS || row >= ROWS || col < 0 || row < 0) {
					cerr << "Fell out of the dungeon: " << row << ", " << col << endl;
					cerr << data_size << endl;
					cerr << pos << endl;
					cerr << data << endl;
					break;
				}
				/* remap ambigous symbols */
				char symbol = data[pos];
				if ((symbol == '|' || symbol == '-') && colour == 33)
					symbol = OPEN_DOOR;
				else if (symbol == '#' && colour == 32)
					symbol = TREE;
				else if (symbol == '#' && colour == 36)
					symbol = IRON_BARS;
				else if (symbol == '\\' && colour == 33)
					symbol = THRONE;
				else if (symbol == '{' && colour != 34) // if it's blue it's a fountain
					symbol = SINK;
				else if (symbol == '}' && colour == 31)
					symbol = LAVA;
				else if (symbol == '.' && colour == 36)
					symbol = ICE;
				else if (symbol == '.' && colour == 33)
					symbol = LOWERED_DRAWBRIDGE;
				else if (colour == 7 && (symbol == '&' || symbol == '\'' || symbol == '6' || symbol == ':' || symbol == ';' || (symbol >= '@' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z') || symbol == '~'))
					symbol = PET;
				map[row][col] = symbol;
				col++;
				break;
		}
	}

	fetchMessages();

	cerr << data << endl;
	//cout << "'" << messages << "'" << endl;

	/* parse attribute & status rows */
	player.parseAttributeRow(map[ATTRIBUTES_ROW]);
	player.parseStatusRow(map[STATUS_ROW]);
	if (row >= MAP_ROW_START && row <= MAP_ROW_END && col >= 0 && col < COLS) {
		/* the last escape sequence *sometimes* place the cursor on the player,
		 * which is quite handy since we won't have to search for the player then */
		map[row][col] = PLAYER;
		player.row = row;
		player.col = col;
	} else if (row == 0) {
		/* cursor on first row? possibly a question? */
		cerr << "POSSIBLY QUESTION" << endl;
		cerr << data << endl;
		/* answer "yes" & carry on */
		command("y");
	} else {
		/* hmm, what else can it be? */
		cerr << "CURSOR ON UNEXPECTED LOCATION: " << row << ", " << col << endl;
		cerr << data << endl;
		sleep(1000);
		exit(42);
	}
}
