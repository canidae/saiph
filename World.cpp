#include "World.h"

/* constructors */
World::World(Connection *connection) {
	this->connection = connection;
	this->row = 0;
	this->col = 0;
	for (int b = 0; b < MESSAGE_BUFFER; ++b) {
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

void World::parsePlayerAttributesAndStatus() {
	/* fetch attributes */
	char alignment[COLS];
	alignment[0] = '\0';
	int matched = sscanf(map[22], "%*[^:]:%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%s", &player.attributes.strength, &player.attributes.dexterity, &player.attributes.constitution, &player.attributes.intelligence, &player.attributes.wisdom, &player.attributes.charisma, alignment);
	if (matched < 7) {
		cerr << "Error parsing attribute line, expected 7 values, found " << matched << endl;
		exit(11);
	}
	if (alignment[0] == 'L')
		player.attributes.alignment = 1;
	else if (alignment[0] == 'N')
		player.attributes.alignment = 0;
	else    
		player.attributes.alignment = -1;

	/* fetch status */
	player.status.encumbrance = 0;
	player.status.hunger = 0;
	player.status.blind = false;
	player.status.confused = false;
	player.status.foodpoisoned = false;
	player.status.hallucinating = false;
	player.status.ill = false;
	player.status.slimed = false;
	player.status.stunned = false;
	char effects[5][COLS];
	matched = sscanf(map[23], "%*[^:]:%d%*[^:]:%d%*[^:]:%d(%d%*[^:]:%d(%d%*[^:]:%d%*[^:]:%d%*[^:]:%d%s%s%s%s%s", &player.status.dungeon, &player.status.zorkmids, &player.status.hitpoints, &player.status.hitpoints_max, &player.status.power, &player.status.power_max, &player.status.armor_class, &player.status.experience, &player.status.turn, effects[0], effects[1], effects[2], effects[3], effects[4]);
	if (matched < 9) {
		cerr << "Error parsing status line, expected at least 9 values, found " << matched << endl;
		exit(12);
	}
	int effects_found = matched - 9;
	for (int a = 0; a < effects_found; ++a) {                                                                                                       
		if (strcmp(effects[a], "Burdened") == 0) {                                                                                              
			player.status.encumbrance = 1;
		} else if (strcmp(effects[a], "Stressed") == 0) {
			player.status.encumbrance = 2;
		} else if (strcmp(effects[a], "Strained") == 0) {
			player.status.encumbrance = 3;
		} else if (strcmp(effects[a], "Overtaxed") == 0) {
			player.status.encumbrance = 4;
		} else if (strcmp(effects[a], "Overloaded") == 0) {
			player.status.encumbrance = 5;
		} else if (strcmp(effects[a], "Fainting") == 0) {
			player.status.hunger = -3;
		} else if (strcmp(effects[a], "Weak") == 0) {                                                                                           
			player.status.hunger = -2;
		} else if (strcmp(effects[a], "Hungry") == 0) {
			player.status.hunger = -1;
		} else if (strcmp(effects[a], "Satiated") == 0) {
			player.status.hunger = 1;
		} else if (strcmp(effects[a], "Oversatiated") == 0) {
			player.status.hunger = 2;
		} else if (strcmp(effects[a], "Blind") == 0) {
			player.status.blind = true;
		} else if (strcmp(effects[a], "Conf") == 0) {
			player.status.confused = true;
		} else if (strcmp(effects[a], "FoodPois") == 0) {
			player.status.foodpoisoned = true;
		} else if (strcmp(effects[a], "Hallu") == 0) {
			player.status.hallucinating = true;
		} else if (strcmp(effects[a], "Ill") == 0) {
			player.status.ill = true;
		} else if (strcmp(effects[a], "Slime") == 0) {
			player.status.slimed = true;
		} else if (strcmp(effects[a], "Stun") == 0) {
			player.status.stunned = true;
		}
	}

	/* fetch position */
	player.position.row = row;
	player.position.col = col;
}

void World::update() {
	/* update the map */
	data_size = connection->retrieve(data, MESSAGE_BUFFER);
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

			case 14:
				/* shift out, invoke G1 character set
				 * dungeon drawing */
				break;

			case 15:
				/* shift in, invoke G0 character set
				 * monsters/object drawing */
				break;

			case 10:
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

	fetchMessages();

	cout << data << endl;

	/* the last escape sequence place the cursor on the player
	 * which is quite handy since we won't have to search for the player then */
	parsePlayerAttributesAndStatus();
}
