#include "World.h"

/* constructors */
World::World(Connection *connection) {
	this->connection = connection;
	memset(view, ' ', sizeof (view));
	for (int r = 0; r < ROWS; ++r)
		view[r][COLS] = '\0';
	memset(color, NO_COLOR, sizeof (color));
	memset(changed, false, sizeof (changed));
	cursor.row = 0;
	cursor.col = 0;
	menu = false;
	question = false;
	memset(messages, '\0', sizeof (messages));
	memset(data, '\0', sizeof (data));
	messages_pos = 0;
	data_size = -1;
	/* fetch the first "frame" */
	update();
}

/* destructors */
World::~World() {
}

/* methods */
bool World::executeCommand(const string &command) {
	/* send a command to nethack */
	for (vector<Point>::iterator c = changes.begin(); c != changes.end(); ++c)
		changed[c->row][c->col] = false;
	changes.clear();
	if (command.size() <= 0) {
		/* huh? no command? */
		return false;
	}
	connection->send(command);
	update();
	return true;
}

/* private methods */
void World::addChangedLocation(const Point &point) {
	/* add a location changed since last frame unless it's already added */
	if (changed[point.row][point.col] || point.row > MAP_ROW_END || point.row < MAP_ROW_BEGIN || point.col > MAP_COL_END || point.col < MAP_COL_BEGIN)
		return;
	changes.push_back(point);
}

void World::fetchMessages() {
	/* new attempt on fetching messages.
	 * this is much better, but still some hacks to get around bugs(?) in nethack.
	 * what do we know?
	 * - "--More--", "(end)" and "(x of y)" is the last chars in data.
	 * - if only 1 line of messages, it's always on first line.
	 * - cursor is always placed after "--More--", "(end)" or "(x of y)" */
	msg_str = &data[data_size - MORE_LENGTH];
	int r = cursor.row;
	int c = cursor.col;
	bool more = false;
	menu = false;
	string::size_type pos = string::npos;
	if ((pos = msg_str.find(MORE, 0)) != string::npos) {
		/* "--More--" found, set c */
		c = cursor.col - MORE_LENGTH;
		more = true;
	} else {
		msg_str = &data[data_size - END_LENGTH];
		if ((pos = msg_str.find(END, 0)) != string::npos) {
			/* "(end)" found, set c.
			 * since there (always?) is an extra space after "(end)",
			 * we'll have to "+ pos" */
			c = cursor.col - END_LENGTH + pos;
			menu = true;
		} else {
			msg_str = &data[data_size - PAGE_LENGTH];
			if ((pos = msg_str.find(PAGE, 0)) != string::npos) {
				/* "(x of y)" found, set c.
				 * this is special, we only search for " of ".
				 * while PAGE_LENGTH covers "y)" we still haven't covered "x)",
				 * so we'll have to move c 2 squares left */
				c = cursor.col - PAGE_LENGTH - 2;
				menu = true;
			} else {
				/* look for question */
				msg_str = &data[data_size - QUESTION_LENGTH];
				question = false;
				if (cursor.row == 0) {
					question = true;
				} else {
					if ((pos = msg_str.find(QUESTION_YN, 0)) == string::npos)
						pos = msg_str.find(QUESTION_YNQ, 0);
					if (pos != string::npos) {
						if ((pos = msg_str.find(QUESTION_DY, 0)) == string::npos)
							if ((pos = msg_str.find(QUESTION_DN, 0)) == string::npos)
								pos = msg_str.find(QUESTION_DQ, 0);
						if (pos != string::npos)
							question = true;
					}
				}

				/* look for messages.
				 * only handles first line, but that should be enough */
				msg_str = view[0];
				string::size_type fns = msg_str.find_first_not_of(" ");
				string::size_type lns = msg_str.find_last_not_of(" ");
				if (fns == string::npos || lns == string::npos || fns >= lns)
					msg_str.clear();
				else
					msg_str = msg_str.substr(fns, lns - fns + 1);
				msg_str.copy(&messages[messages_pos], msg_str.length());
				messages[messages_pos + msg_str.length()] = '\0';
				messages_pos = 0;
				return; // no messages or messages on 1 line
			}
		}
	}
	if (r == 0) {
		/* not a menu, remove "--More--" from end of line */
		msg_str = view[r];
		msg_str = msg_str.substr(0, c);
		/* append 2 spaces for later splitting */
		msg_str.append(2, ' ');
		msg_str.copy(&messages[messages_pos], msg_str.length());
		messages_pos += msg_str.length();
		messages[messages_pos] = '\0';
	} else {
		/* list, add all lines to msg_str, splitted by "  "
		 * no point adding last row, it just contain "--More--", "(end)" or "(1 of 2)" */
		for (int r2 = 0; r2 < r; ++r2) {
			msg_str = &view[r2][c];
			/* trim */
			string::size_type fns = msg_str.find_first_not_of(" ");
			string::size_type lns = msg_str.find_last_not_of(" ");
			if (fns == string::npos || lns == string::npos || fns >= lns)
				continue; // blank line?
			msg_str = msg_str.substr(fns, lns - fns + 1);
			/* append 2 spaces for later splitting */
			msg_str.append(2, ' ');
			msg_str.copy(&messages[messages_pos], msg_str.length());
			messages_pos += msg_str.length();
			messages[messages_pos] = '\0';
		}
	}
	if (more) {
		/* there are "--More--" messages.
		 * since this is not a menu we'll just ask for the rest of the messages.
		 * also, don't call executeCommand(), because that clears the "changes" vector */
		connection->send(" ");
		update();
		return;
	}
}

void World::handleEscapeSequence(int *pos, int *color) {
	if (data[*pos] == 27) {
		/* sometimes we get 2 escape chars in a row,
		 * just return in those cases */
		return;
	} else if (data[*pos] == '[') {
		int divider = -1;
		int start = *pos;
		for (; *pos < data_size; ++*pos) {
			if (data[*pos] == ';') {
				/* divider for values */
				divider = *pos;
			} else if (data[*pos] == 'A') {
				/* move cursor up */
				if (cursor.row > 0)
					--cursor.row;
				break;
			} else if (data[*pos] == 'B') {
				/* move cursor down */
				if (cursor.row < ROWS)
					++cursor.row;
				break;
			} else if (data[*pos] == 'C') {
				/* move cursor right */
				if (cursor.col < COLS)
					++cursor.col;
				break;
			} else if (data[*pos] == 'D') {
				/* move cursor left */
				if (cursor.col > 0)
					--cursor.col;
				break;
			} else if (data[*pos] == 'H') {
				/* set cursor position */
				cursor.row = 0;
				cursor.col = 0;
				if (divider < 0)
					break;
				/* we got a position */
				int matched = sscanf(&data[start + 1], "%d;%d", &cursor.row, &cursor.col);
				--cursor.row; // terminal starts counting from 1
				--cursor.col; // ditto ^^
				if (matched < 2) {
					cerr << "Unable to place cursor" << endl;
					cerr << &data[start] << endl;
					exit(13);
				}
				break;
			} else if (data[*pos] == 'J') {
				/* erase in display */
				if (data[*pos - 1] == '[') {
					/* erase everything below current position */
					for (int r = cursor.row + 1; r < ROWS; ++r) {
						for (int c = 0; c < COLS; ++c)
							view[r][c] = ' ';
					}
				} else if (data[*pos - 1] == '1') {
					/* erase everything above current position */
					for (int r = cursor.row - 1; r >= 0; --r) {
						for (int c = 0; c < COLS; ++c)
							view[r][c] = ' ';
					}
				} else if (data[*pos - 1] == '2') {
					/* erase entire display */
					memset(view, ' ', sizeof (view));
					for (int r = 0; r < ROWS; ++r)
						view[r][COLS] = '\0';
					cursor.row = 0;
					cursor.col = 0;
					*color = 0;
				} else {
					cerr << "Unhandled sequence: " << endl;
					cerr << &data[*pos] << endl;
					exit(9);
				}
				break;
			} else if (data[*pos] == 'K') {
				/* erase in line */
				if (data[*pos - 1] == '[') {
					/* erase everything to the right */
					for (int c = cursor.col; c < COLS; ++c)
						view[cursor.row][c] = ' ';
				} else if (data[*pos - 1] == '1') {
					/* erase everything to the left */
					for (int c = 0; c < cursor.col; ++c)
						view[cursor.row][c] = ' ';
				} else if (data[*pos - 1] == '2') {
					/* erase entire line */
					for (int c = 0; c < COLS; ++c)
						view[cursor.row][c] = ' ';
				} else {
					cerr << "Unhandled sequence: " << endl;
					cerr << &data[*pos] << endl;
					exit(9);
				}
				break;
			} else if (data[*pos] == 'h') {
				/* can possibly be ignored */
				/* probably [?1049h */
				break;
			} else if (data[*pos] == 'l') {
				/* DEC Private Mode Reset? :s */
				break;
			} else if (data[*pos] == 'm') {
				/* character attribute (bold, inverted, color, etc) */
				if (divider > 0) {
					cerr << "Unsupported character color" << endl;
					cerr << &data[*pos] << endl;
					exit(15);
					break;
				}
				*color = 0;
				if (*pos == start + 1)
					break;
				int value = 0;
				int matched = sscanf(&data[start + 1], "%d", &value);
				if (matched < 1) {
					cerr << "Expected numeric value for character attribute" << endl;
					cerr << &data[*pos] << endl;
					exit(14);
				}
				*color = value;
				break;
			} else if (data[*pos] == 'r') {
				/* this is some scrolling crap, ignore it */
				break;
			} else if (data[*pos] == 27) {
				/* escape char found, that shouldn't happen */
				cerr << "Escape character found in sequence: " << endl;
				for (int a = start; a <= *pos; ++a)
					cerr << (int) data[a] << " - ";
				cerr << endl;
				cerr << &data[start] << endl;
				cerr << data << endl;
				exit(7);
			} else if (*pos - start > 7) {
				/* too long escape sequence? */
				cerr << "Suspiciously long sequence: " << endl;
				cerr << &data[*pos] << endl;
				exit(8);
			}
		}
		if (*pos >= data_size) {
			cerr << "Did not find stop char for sequence" << endl;
			cerr << data << endl;
			exit(6);
		}
	} else if (data[*pos] == '(') {
		/* designate character set, ignore */
		++*pos;
	} else if (data[*pos] == ')') {
		/* designate character set, ignore */
		++*pos;
	} else if (data[*pos] == '*') {
		/* designate character set, ignore */
		++*pos;
	} else if (data[*pos] == '+') {
		/* designate character set, ignore */
		++*pos;
	} else if (data[*pos] == 'M') {
		/* reverse linefeed? */
		if (cursor.row > 0)
			--cursor.row;
	} else if (data[*pos] == '=') {
		/* application numpad?
		 * ignore */
	} else if (data[*pos] == '>') {
		/* normal numpad?
		 * ignore */
	} else {
		cerr << "Unsupported escape sequence code at char " << *pos << ": ";
		cerr << &data[*pos] << endl;
		exit(5);
	}
}

void World::update() {
	/* update the view */
	int color = 0; // color of the char
	data_size = connection->retrieve(data, BUFFER_SIZE);
	/* print world
	 * this must be done here because if we get --More-- messages we'll update again */
	cout << data;
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
				if (cursor.col > 0)
					--cursor.col;
				break;

			case 10:
				/* line feed */
				++cursor.row;
				break;

			case 13:
				/* carriage return */
				cursor.col = 0;
				break;

			case 14:
				/* shift out, invoke G1 character set */
				break;

			case 15:
				/* shift in, invoke G0 character set */
				break;

			case 27:
				/* escape sequence coming up */
				++pos;
				handleEscapeSequence(&pos, &color);
				break;

			default:
				/* add this char to the view */
				if (cursor.col >= COLS || cursor.row >= ROWS || cursor.col < 0 || cursor.row < 0) {
					cerr << "Fell out of the dungeon: " << cursor.row << ", " << cursor.col << endl;
					cerr << data_size << endl;
					cerr << pos << endl;
					cerr << data << endl;
					break;
				}
				view[cursor.row][cursor.col] = data[pos];
				addChangedLocation(cursor);
				this->color[cursor.row][cursor.col] = color;
				cursor.col++;
				break;
		}
	}

	fetchMessages();

	/* parse attribute & status rows */
	bool parsed_attributes = player.parseAttributeRow(view[ATTRIBUTES_ROW]);
	bool parsed_status = player.parseStatusRow(view[STATUS_ROW]);
	if (parsed_attributes && parsed_status && cursor.row >= MAP_ROW_BEGIN && cursor.row <= MAP_ROW_END && cursor.col >= MAP_COL_BEGIN && cursor.col <= MAP_COL_END) {
		/* the last escape sequence *sometimes* place the cursor on the player,
		 * which is quite handy since we won't have to search for the player then */
		view[cursor.row][cursor.col] = PLAYER;
		player.row = cursor.row;
		player.col = cursor.col;
	} else if (!menu && !question) {
		/* hmm, what else can it be?
		 * could we be missing data?
		 * this is bad, we'll lose messages, this should never happen */
		cerr << "CURSOR ON UNEXPECTED LOCATION: " << cursor.row << ", " << cursor.col << endl;
		cerr << data << endl;
		update();
		return;
	}
}
