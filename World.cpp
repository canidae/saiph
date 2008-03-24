#include "World.h"

/* constructors */
World::World() {
	this->map = new Map[HISTORY];
	this->map_current = 0;

	for (int a = 0; a < HISTORY; ++a) {
		this->map[a].data = new char[BUFFER];
		this->map[a].data[0] = '\0';
		this->map[a].size = -1;
	}

	/* set up pipes */
	if (pipe(input) < 0 || pipe(output) < 0) {
		cerr << "Plumbing failed" << endl;
		exit(1);
	}

	/* set up pty */
	int fd;
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
	for (int a = 0; a < HISTORY; ++a)
		delete [] map[a].data;
	delete [] map;
}

/* methods */
void World::command(char *command) {
	/* send a command to nethack */
	write(input[1], command, strlen(command));
	update();
}

Map World::getMap() {
	/* fetch the map */
	return map[map_current];
}

void World::update() {
	/* update the map */
	usleep(USLEEP); // sleep a bit so we're sure the map is updated
	map_current = ++map_current % HISTORY;
	map[map_current].size = read(output[0], map[map_current].data, BUFFER);
	/* reset map */
	for (int row = 0; row < ROWS; ++row) {
		for (int col = 0; col < COLS; ++col)
			map[map_current].map[row][col] = ' ';
	}
	int row = 0;
	int col = 0;
	for (int pos = 0; pos < map[map_current].size; ++pos) {
		switch (map[map_current].data[pos]) {
			case 27:
				/* escape sequence coming up */
				handleEscapeSequence(++pos, row, col);
				break;

			default:
				/* add this char to the map */
				if (col >= COLS || row >= ROWS) {
					cerr << "Fell out of the dungeon" << endl;
					exit(4);
				}
				map[map_current].map[row][col] = map[map_current].data[pos];
				col++;
				if (col >= COLS) {
					col = 0;
					row++;
				}
				break;
		}
	}
}

void World::handleEscapeSequence(int &pos, int &row, int &col) {
	char *d = map[map_current].data;
	int divider = -1;
	if (d[pos] == '[') {
		int start = pos;
		for (; pos < map[map_current].size; ++pos) {
			if (d[pos] == ';') {
				/* divider for values */
				divider = pos;
			} else if (d[pos] == 'h') {
				/* can possibly be ignored */
				break;
			} else if (d[pos] == 'J') {
				/* erase in display */
				break;
			} else if (d[pos] == 'H') {
				/* set cursor position */
				break;
			} else if (d[pos] == 'K') {
				/* erase a direction */
				break;
			}
		}
		if (pos >= map[map_current].size) {
			cerr << "Did not find stop char for sequence" << endl;
			for (int a = start; a < pos; ++a)
				cerr << map[map_current].data[a];
			cerr << endl;
			exit(6);
		}
	} else if (d[pos] == '(') {
		/* designate character set, ignore */
		++pos;
	} else if (d[pos] == ')') {
		/* designate character set, ignore */
		++pos;
	} else {
		cerr << "Unsupported escape sequence code at char " << pos << ": ";
		for (int a = pos; a < pos + 7 || a >= map[map_current].size; ++a)
			cerr << map[map_current].data[a];
		cerr << endl;
		exit(5);
	}
}
