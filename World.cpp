#include "World.h"

/* constructors */
World::World() {
	for (int b = 0; b < BUFFER; ++b)
		this->map.data[b] = '\0';
	this->map.data_size = -1;
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c)
			this->map.map[r][c] = ' ';
		this->map.map[r][80] = '\0';
	}

	/* set up pipes */
	if (pipe(input) < 0 || pipe(output) < 0) {
		cerr << "Plumbing failed" << endl;
		exit(1);
	}

	/* set up pty */
	int fd = 0;
	char slave[256];
	slave[0] = '\0';
	winsize wsize;
	wsize.ws_row = ROWS;
	wsize.ws_col = COLS;
	wsize.ws_xpixel = 640;
	wsize.ws_ypixel = 480;
	pid_t pid = forkpty(&fd, slave, NULL, &wsize);
	if (pid == -1) {
		cerr << "There is no fork" << endl;
		exit(2);
	} else if (pid) {
		/* fix plumbing */
		input[1] = fd; // input == data sent to nethack
		output[0] = fd; // output == data received from nethack
		/* make reading from output pipe non-blocking */
		int result = fcntl(output[0], F_GETFL);
		result |= O_NONBLOCK;
		fcntl(output[0], F_SETFL, result);
		update();
	} else {
		/* start nethack here */
		int result = execl(NETHACK, NETHACK, NULL);
		if (result < 0) {
			cerr << "Unable to enter the dungeon" << endl;
			exit(3);
		}
	}
}

/* destructors */
World::~World() {
}

/* methods */
void World::command(char *command) {
	/* send a command to nethack */
	write(input[1], command, strlen(command));
	update();
}

/* private methods */
void World::handleEscapeSequence(int &pos, int &row, int &col) {
	char *d = map.data;
	if (d[pos] == 27) {
		/* sometimes we get 2 escape chars in a row
		 * just return in those cases */
		return;
	} else if (d[pos] == '[') {
		int divider = -1;
		int start = pos;
		for (; pos < map.data_size; ++pos) {
			if (d[pos] == ';') {
				/* divider for values */
				divider = pos;
			} else if (d[pos] == 'A') {
				/* move cursor up */
				if (row > 0)
					--row;
				break;
			} else if (d[pos] == 'B') {
				/* move cursor down */
				if (row < ROWS)
					++row;
				break;
			} else if (d[pos] == 'C') {
				/* move cursor right */
				if (col < COLS)
					++col;
				break;
			} else if (d[pos] == 'D') {
				/* move cursor left */
				if (col > 0)
					--col;
				break;
			} else if (d[pos] == 'H') {
				/* set cursor position */
				row = 0;
				col = 0;
				if (divider > 0) {
					/* we got a position */
					if (divider - start == 3) {
						/* two digits in row */
						row = row + ((int) d[divider - 2] - '0') * 10;
					}
					row = row + ((int) d[divider - 1] - '0') - 1;
					if (pos - divider == 3) {
						/* two digits in col */
						col = col + ((int) d[pos - 2] - '0') * 10;
					}
					col = col + ((int) d[pos - 1] - '0') - 1;
				}
				break;
			} else if (d[pos] == 'J') {
				/* erase in display */
				if (d[pos - 1] == '[') {
					/* erase everything below current position */
					for (int r = row + 1; r < ROWS; ++r) {
						for (int c = 0; c < COLS; ++c) {
							map.map[r][c] = ' ';
						}
					}
				} else if (d[pos - 1] == '1') {
					/* erase everything aboce current position */
					for (int r = row - 1; r >= 0; --r) {
						for (int c = 0; c < COLS; ++c) {
							map.map[r][c] = ' ';
						}
					}
				} else if (d[pos - 1] == '2') {
					/* erase entire display */
					for (int r = 0; r < ROWS; ++r) {
						for (int c = 0; c < COLS; ++c) {
							map.map[r][c] = ' ';
						}
					}
					row = 0;
					col = 0;
				} else {
					cerr << "Unhandled sequence: " << endl;
					cerr << &map.data[pos] << endl;
					exit(9);
				}
				break;
			} else if (d[pos] == 'K') {
				/* erase in line */
				if (d[pos - 1] == '[') {
					/* erase everything to the right */
					for (int c = col + 1; c < COLS; ++c) {
						map.map[row][c] = ' ';
					}
				} else if (d[pos - 1] == '1') {
					/* erase everything to the left */
					for (int c = 0; c < col; ++c) {
						map.map[row][c] = ' ';
					}
				} else if (d[pos - 1] == '2') {
					/* erase entire line */
					for (int c = 0; c < COLS; ++c) {
						map.map[row][c] = ' ';
					}
				} else {
					cerr << "Unhandled sequence: " << endl;
					cerr << &map.data[pos] << endl;
					exit(9);
				}
				break;
			} else if (d[pos] == 'h') {
				/* can possibly be ignored */
				/* probably [?1049h */
				break;
			} else if (d[pos] == 'm') {
				/* character attribute (bold, inverted, etc) */
				break;
			} else if (d[pos] == 27) {
				/* escape char found, that shouldn't happen */
				cerr << "Escape character found in sequence: " << endl;
				for (int a = start; a <= pos; ++a)
					cerr << (int) map.data[a] << " - ";
				cerr << endl;
				cerr << &map.data[start] << endl;
				cerr << map.data << endl;
				exit(7);
			} else if (pos - start > 7) {
				/* too long escape sequence? */
				cerr << "Suspiciously long sequence: " << endl;
				cerr << &map.data[pos] << endl;
				exit(8);
			}
		}
		if (pos >= map.data_size) {
			cerr << "Did not find stop char for sequence" << endl;
			cerr << &map.data[pos] << endl;
			exit(6);
		}
	} else if (d[pos] == '(') {
		/* designate character set, ignore */
		++pos;
	} else if (d[pos] == ')') {
		/* designate character set, ignore */
		++pos;
	} else if (d[pos] == 'M') {
		/* reverse linefeed? */
		if (row > 0)
			--row;
		++pos;
	} else {
		cerr << "Unsupported escape sequence code at char " << pos << ": ";
		cerr << &map.data[pos] << endl;
		exit(5);
	}
}

void World::update() {
	/* update the map */
	usleep(USLEEP); // sleep a bit so we're sure the map is updated
	map.data_size = read(output[0], map.data, BUFFER);
	/* reset map */
	for (int row = 0; row < ROWS; ++row) {
		for (int col = 0; col < COLS; ++col)
			map.map[row][col] = ' ';
	}
	int row = 0;
	int col = 0;
	for (int pos = 0; pos < map.data_size; ++pos) {
		switch (map.data[pos]) {
			case 27:
				/* escape sequence coming up */
				handleEscapeSequence(++pos, row, col);
				break;

			case 8:
			case 10:
			case 13:
			case 14:
			case 15:
				/* various control characters we'll ignore */
				break;

			default:
				/* add this char to the map */
				if (col >= COLS || row >= ROWS || col < 0 || row < 0) {
					cerr << "Fell out of the dungeon: " << row << ", " << col << endl;
					cerr << &map.data[pos] << endl;
					exit(4);
				}
				map.map[row][col] = map.data[pos];
				col++;
				break;
		}
	}
	/* the last escape sequence place the cursor on the player
	 * which is quite handy since we won't have to search for the player then */
}
