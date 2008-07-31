#include "World.h"

/* constructors */
World::World(Connection *connection, ofstream *debugfile) : connection(connection), debugfile(debugfile) {
	memset(view, ' ', sizeof (view));
	for (int r = 0; r < ROWS; ++r)
		view[r][COLS] = '\0';
	memset(color, NO_COLOR, sizeof (color));
	memset(changed, false, sizeof (changed));
	cursor.row = 0;
	cursor.col = 0;
	messages = "  ";
	cur_page = -1;
	max_page = -1;
	menu = false;
	question = false;
	last_menu = Point(-1, -1);
	memset(data, '\0', sizeof (data));
	data_size = -1;
        /* remapping of unique symbols */
        for (int s = 0; s <= UCHAR_MAX; ++s) {
                for (int c = 0; c <= CHAR_MAX; ++c)
                        uniquemap[s][c] = s;
        }
        uniquemap[(unsigned char) CORRIDOR][CYAN] = IRON_BARS;
        uniquemap[(unsigned char) CORRIDOR][GREEN] = TREE;
        uniquemap[(unsigned char) FLOOR][CYAN] = ICE;
        uniquemap[(unsigned char) FLOOR][YELLOW] = LOWERED_DRAWBRIDGE;
        uniquemap[(unsigned char) FOUNTAIN][NO_COLOR] = SINK;
        uniquemap[(unsigned char) GRAVE][YELLOW] = THRONE;
        uniquemap[(unsigned char) HORIZONTAL_WALL][YELLOW] = OPEN_DOOR;
        uniquemap[(unsigned char) VERTICAL_WALL][YELLOW] = OPEN_DOOR;
        uniquemap[(unsigned char) WATER][RED] = LAVA;
	/* fetch the first "frame" */
	update();
}

/* methods */
bool World::executeCommand(const string &command) {
	/* send a command to nethack */
	for (vector<Point>::iterator c = changes.begin(); c != changes.end(); ++c)
		changed[c->row][c->col] = false;
	changes.clear();
	messages = "  "; // we want 2 spaces before the first message too
	if (command.size() <= 0) {
		/* huh? no command? */
		return false;
	}
	connection->transmit(command);
	update();
	return true;
}

/* private methods */
void World::addChangedLocation(const Point &point) {
	/* add a location changed since last frame unless it's already added */
	if (changed[point.row][point.col] || point.row < MAP_ROW_BEGIN || point.row > MAP_ROW_END || point.col < MAP_COL_BEGIN || point.col > MAP_COL_END)
		return;
	changes.push_back(point);
}

void World::fetchMenuText(int stoprow, int startcol, bool addspaces) {
	/* fetch text from a menu */
	for (int r = 0; r <= stoprow; ++r) {
		msg_str = &view[r][startcol];
		/* trim */
		string::size_type fns = msg_str.find_first_not_of(" ");
		string::size_type lns = msg_str.find_last_not_of(" ");
		if (fns == string::npos || lns == string::npos || fns >= lns)
			continue; // blank line?
		msg_str = msg_str.substr(fns, lns - fns + 1);
		if (addspaces)
			msg_str.append(2, ' '); // append 2 spaces for later splitting
		messages.append(msg_str);
	}
}

void World::fetchMessages() {
	/* even yet a try on fetching messages sanely */
	question = false; // we can do this as a question max last 1 turn
	msg_str = &data[data_size - sizeof (MORE)];
	string::size_type pos = string::npos;
	if ((pos = msg_str.find(MORE, 0)) != string::npos) {
		/* "--More--" found */
		menu = false; // we don't have a menu then
		int r = cursor.row;
		int c = cursor.col - sizeof (MORE) + 1; // +1 because sizeof (MORE) is 9, not 8
		if (r == 0) {
			/* only one line, remove "--More--" from end of line */
			msg_str = view[r];
			msg_str = msg_str.substr(0, c);
			/* append 2 spaces for later splitting */
			msg_str.append(2, ' ');
			messages.append(msg_str);
		} else {
			/* more than 1 line */
			if (c == 0 || view[r][c - 1] != ' ') {
				/* this is just a very long line, not a list */
				c = 0;
				fetchMenuText(r, c, false);
				messages.erase(messages.size() - sizeof (MORE) + 1); // remove "--More--"
				messages.append(2, ' '); // add two spaces
			} else {
				/* this is a list */
				/* sometimes "--More--" is placed 1 char to the right of the menu.
				 * this happens at least when the entire page is filled.
				 * check that the line above also is ' ', if not, c - 1 */
				if (view[r - 1][c - 1] == ' ')
					fetchMenuText(r - 1, c, true); // "r - 1" to avoid the last "--More--"
				else
					fetchMenuText(r - 1, c - 1, true); // "r - 1" to avoid the last "--More--"
			}
		}
		/* request the remaining messages */
		connection->transmit(" ");
		update();
		return;
	} else if (cursor.row == 0) {
		/* looks like we got a question.
		 * we might want to significantly improve this later,
		 * as we sometimes get partial data */
		question = true;
		menu = false; // no menu when we got a question
	} else {
		/* --More-- not found, but we might have a menu.
		 * this is pain */
		if (menu) {
			/* we had a menu last frame, check if we still do */
			msg_str = &view[last_menu.row][last_menu.col];
			int x, y;
			if (msg_str.find(END, 0) == string::npos && sscanf(&view[last_menu.row][last_menu.col], PAGE, &x, &y) != 2) {
				/* nah, last menu is gone */
				menu = false;
				last_menu.row = -1;
				last_menu.col = -1;
			}
		}
		if (!menu) {
			/* check if we got a new menu */
			msg_str = &data[data_size - sizeof (PAGE)];
			cur_page = -1;
			max_page = -1;
			if (msg_str.find(END, 0) != string::npos || sscanf(msg_str.c_str(), PAGE, &cur_page, &max_page) == 2) {
				/* hot jiggity! we got a list */
				/* now find the "(" in "(end) " or "(x of y)" */
				if (cur_page == -1) {
					/* only 1 page */
					cur_page = 1;
					max_page = 1;
				}
				int c;
				for (c = cursor.col; c >= 0 && view[cursor.row][c] != '('; --c)
					;
				menu = true;
				last_menu.row = cursor.row;
				last_menu.col = c;
			}
		}
		if (menu) {
			/* finally parse the menu */
			fetchMenuText(last_menu.row - 1, last_menu.col, true); // "r - 1" to avoid the last "(end) " or "(x of y)"
		}
	}
	if (!menu) {
		/* no "--More--", no question and no menu?
		 * well, it gotta be no messages or the message is on 1 line, then */
		msg_str = view[0];
		/* trim */
		string::size_type fns = msg_str.find_first_not_of(" ");
		string::size_type lns = msg_str.find_last_not_of(" ");
		if (fns == string::npos || lns == string::npos || fns >= lns)
			return; // blank line?
		msg_str = msg_str.substr(fns, lns - fns + 1);
		messages.append(msg_str);
		messages.append(2, ' ');
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
					*debugfile << WORLD_DEBUG_NAME << "Unable to place cursor: " << &data[start] << endl;
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
					*debugfile << WORLD_DEBUG_NAME << "Unhandled sequence: " << &data[*pos] << endl;
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
					*debugfile << WORLD_DEBUG_NAME << "Unhandled sequence: " << &data[*pos] << endl;
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
					*debugfile << WORLD_DEBUG_NAME << "Unsupported character color" << &data[*pos] << endl;
					exit(15);
					break;
				}
				*color = 0;
				if (*pos == start + 1)
					break;
				int value = 0;
				int matched = sscanf(&data[start + 1], "%d", &value);
				if (matched < 1) {
					*debugfile << WORLD_DEBUG_NAME << "Expected numeric value for character attribute: " << &data[*pos] << endl;
					exit(14);
				}
				*color = value;
				break;
			} else if (data[*pos] == 'r') {
				/* this is some scrolling crap, ignore it */
				break;
			} else if (data[*pos] == 27) {
				/* escape char found, that shouldn't happen */
				*debugfile << WORLD_DEBUG_NAME << "Escape character found in sequence: ";
				for (int a = start; a <= *pos; ++a)
					*debugfile << (int) data[a] << " - ";
				*debugfile << endl;
				exit(7);
			} else if (*pos - start > 7) {
				/* too long escape sequence? */
				*debugfile << WORLD_DEBUG_NAME << "Suspiciously long sequence: " << &data[*pos] << endl;
				exit(8);
			}
		}
		if (*pos >= data_size) {
			*debugfile << WORLD_DEBUG_NAME << "Did not find stop char for sequence: " << data << endl;
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
		*debugfile << WORLD_DEBUG_NAME << "Unsupported escape sequence code at char " << *pos << ": " << &data[*pos] << endl;
		exit(5);
	}
}

void World::update() {
	/* update the view */
	int color = 0; // color of the char
	data_size = connection->retrieve(data, BUFFER_SIZE);
	/* print world & data (to cerr, for debugging)
	 * this must be done here because if we get --More-- messages we'll update again */
	/* also, we do this in two loops because otherwise it flickers a lot */
	for (int a = 0; a < data_size; ++a)
		cout << data[a];
	cout.flush(); // same reason as in saiph.dumpMaps()
	*debugfile << DATA_DEBUG_NAME;
	for (int a = 0; a < data_size; ++a)
		*debugfile << data[a];
	*debugfile << endl;
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
					*debugfile << WORLD_DEBUG_NAME << "Fell out of the dungeon: " << cursor.row << ", " << cursor.col << endl;
					break;
				}
				view[cursor.row][cursor.col] = uniquemap[(unsigned char) data[pos]][color];
				this->color[cursor.row][cursor.col] = color;
				addChangedLocation(cursor);
				cursor.col++;
				break;
		}
	}

	fetchMessages();

	/* parse attribute & status rows */
	bool parsed_attributes = player.parseAttributeRow(view[ATTRIBUTES_ROW]);
	bool parsed_status = player.parseStatusRow(view[STATUS_ROW]);
	if (parsed_attributes && parsed_status && cursor.row >= MAP_ROW_BEGIN && cursor.row <= MAP_ROW_END && cursor.col >= MAP_COL_BEGIN && cursor.col <= MAP_COL_END && !menu && !question) {
		/* the last escape sequence *sometimes* place the cursor on the player,
		 * which is quite handy since we won't have to search for the player then */
		view[cursor.row][cursor.col] = PLAYER;
		player.row = cursor.row;
		player.col = cursor.col;
	} else if (!menu && !question) {
		/* hmm, what else can it be?
		 * could we be missing data?
		 * this is bad, we'll lose messages, this should never happen */
		*debugfile << WORLD_DEBUG_NAME << "CURSOR ON UNEXPECTED LOCATION: " << cursor.row << ", " << cursor.col << endl;
		update();
		return;
	}
	if (messages == "  ")
		messages.clear(); // no messages
}
