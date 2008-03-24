#include "World.h"

/* constructors */
World::World() {
	this->row = 0;
	this->col = 0;
	for (int b = 0; b < BUFFER; ++b)
		this->data[b] = '\0';
	this->data_size = -1;
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c)
			this->map[r][c] = ' ';
		this->map[r][80] = '\0';
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
void World::handleEscapeSequence(int &pos) {
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
				if (divider > 0) {
					/* we got a position */
					if (divider - start == 3) {
						/* two digits in row */
						row += ((int) data[divider - 2] - '0') * 10;
					}
					row = row + ((int) data[divider - 1] - '0') - 1;
					if (pos - divider == 3) {
						/* two digits in col */
						col += ((int) data[pos - 2] - '0') * 10;
					}
					col = col + ((int) data[pos - 1] - '0') - 1;
				}
				break;
			} else if (data[pos] == 'J') {
				/* erase in display */
				if (data[pos - 1] == '[') {
					/* erase everything below current position */
					for (int r = row + 1; r < ROWS; ++r) {
						for (int c = 0; c < COLS; ++c) {
							map[r][c] = ' ';
						}
					}
				} else if (data[pos - 1] == '1') {
					/* erase everything aboce current position */
					for (int r = row - 1; r >= 0; --r) {
						for (int c = 0; c < COLS; ++c) {
							map[r][c] = ' ';
						}
					}
				} else if (data[pos - 1] == '2') {
					/* erase entire display */
					for (int r = 0; r < ROWS; ++r) {
						for (int c = 0; c < COLS; ++c) {
							map[r][c] = ' ';
						}
					}
					row = 0;
					col = 0;
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
					for (int c = col + 1; c < COLS; ++c) {
						map[row][c] = ' ';
					}
				} else if (data[pos - 1] == '1') {
					/* erase everything to the left */
					for (int c = 0; c < col; ++c) {
						map[row][c] = ' ';
					}
				} else if (data[pos - 1] == '2') {
					/* erase entire line */
					for (int c = 0; c < COLS; ++c) {
						map[row][c] = ' ';
					}
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
				/* character attribute (bold, inverted, etc) */
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
	} else if (data[pos] == 'M') {
		/* reverse linefeed? */
		if (row > 0)
			--row;
		++pos;
	} else {
		cerr << "Unsupported escape sequence code at char " << pos << ": ";
		cerr << &data[pos] << endl;
		exit(5);
	}
}

void World::update() {
	/* update the map */
	usleep(USLEEP); // sleep a bit so we're sure the map is updated
	data_size = read(output[0], data, BUFFER);
	/* reset map */
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c)
			map[r][c] = ' ';
	}
	for (int pos = 0; pos < data_size; ++pos) {
		switch (data[pos]) {
			case 27:
				/* escape sequence coming up */
				handleEscapeSequence(++pos);
				break;

			case 13:
				/* carriage return.
				 * make it go to beginning of line */
				col = 0;
				break;

			case 8:
				/* backspace.
				 * make it go 1 char left */
				if (col > 0)
					--col;
				break;

			case 10:
			case 14:
			case 15:
				/* various control characters we'll ignore */
				break;

			default:
				/* add this char to the map */
				if (col >= COLS || row >= ROWS || col < 0 || row < 0) {
					cerr << "Fell out of the dungeon: " << row << ", " << col << endl;
					cerr << &data[pos] << endl;
					exit(4);
				}
				map[row][col] = data[pos];
				col++;
				break;
		}
	}
	/* the last escape sequence place the cursor on the player
	 * which is quite handy since we won't have to search for the player then */
}
