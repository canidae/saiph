#include "World.h"

/* constructors */
World::World(Connection *connection) {
	this->connection = connection;
	this->row = 0;
	this->col = 0;
	for (int b = 0; b < BUFFER_SIZE; ++b) {
		this->data[b] = '\0';
		this->messages[b] = '\0';
	}
	this->data_size = -1;
	this->messages_pos = 0;
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c)
			this->map[r][c] = ' ';
		this->map[r][80] = '\0';
	}
	/* fetch the first "frame" */
	update();
}

/* destructors */
World::~World() {
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
		/* sometimes we get 2 escape chars in a row
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
					for (int c = col + 1; c < COLS; ++c)
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
				if (value >= 30)
					colour = value;
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
			cerr << &data[pos] << endl;
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
	} else {
		cerr << "Unsupported escape sequence code at char " << pos << ": ";
		cerr << &data[pos] << endl;
		exit(5);
	}
}

void World::fetchMessages() {
	/* fetch the messages currently displayed in map */
	for (int a = 0; a < COLS; ++a)
		messages[a + messages_pos] = map[0][a];
	messages_pos += COLS;
	/* if first line ends with "--More--" we'll command(" ") for the rest of the message */
	if (strstr(map[0], MORE))
		command(" "); // request "next page"
	/* if there are several items here we may get a menu.
	 * so we'll have to look for "--More--" on the other lines. */
	for (int r = 1; r < ROWS - 2; ++r) {
		string str(map[r]);
		string::size_type pos = str.find(MORE, 0);
		if (pos == string::npos)
			pos = str.find(END, 0);
		if (pos != string::npos) {
			/* found "--More--" or "(end)" */
			for (int r2 = 1; r2 < r; ++r) {
				for (int c = pos; c < COLS; ++c)
					messages[c + messages_pos] = map[r2][c];
				messages_pos += COLS - pos;
			}
			command(" "); // requeste "next page"
			break;
		}
	}
	messages_pos = 0;
}

void World::update() {
	/* update the map */
	int colour = 0; // colour of the char
	data_size = connection->retrieve(data, BUFFER_SIZE);
	for (int pos = 0; pos < data_size; ++pos) {
		switch (data[pos]) {
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
					cerr << &data[pos] << endl;
					exit(4);
				}
				/* remap ambigous symbols */
				if ((data[pos] == '|' || data[pos] == '-') && colour == 33)
					map[row][col] = OPEN_DOOR;
				else if (data[pos] == '#' && colour == 32)
					map[row][col] = TREE;
				else if (data[pos] == '#' && colour == 36)
					map[row][col] = IRON_BARS;
				else if (data[pos] == '\\' && colour == 33)
					map[row][col] = THRONE;
				else if (data[pos] == '{' && colour != 34) // if it's blue it's a fountain
					map[row][col] = SINK;
				else if (data[pos] == '}' && colour == 31)
					map[row][col] = LAVA;
				else if (data[pos] == '.' && colour == 36)
					map[row][col] = ICE;
				else if (data[pos] == '.' && colour == 33)
					map[row][col] = LOWERED_DRAWBRIDGE;
				else
					map[row][col] = data[pos];
				col++;
				break;
		}
	}

	fetchMessages();

	cout << data << endl;

	/* parse attribute & status rows */
	player.parseAttributeRow(map[ATTRIBUTES_ROW]);
	player.parseStatusRow(map[STATUS_ROW]);
	/* the last escape sequence place the cursor on the player
	 * which is quite handy since we won't have to search for the player then */
	player.row = row;
	player.col = col;
}
