#include "Saiph.h"

/* constructors */
Saiph::Saiph(int interface) {
	debugfile.open("debug.log", ios::trunc);
	connection = Connection::create(interface, &debugfile);
	if (connection == NULL) {
		cout << "ERROR: Don't know what interface this is: " << interface << endl;
		exit(1);
	}
	world = new World(connection, &debugfile);

	/* engulfed */
	engulfed = false;

	/* set best_priority to ILLEGAL_PRIORITY */
	best_priority = ILLEGAL_PRIORITY;

	/* set on_ground to NULL */
	on_ground = NULL;

	/* Analyzers */
	analyzers.push_back(new Beatitude(this));
	analyzers.push_back(new Door(this));
	analyzers.push_back(new Elbereth(this));
	analyzers.push_back(new Enhance(this));
	analyzers.push_back(new Excalibur(this));
	analyzers.push_back(new Explore(this));
	analyzers.push_back(new Fight(this));
	analyzers.push_back(new Food(this));
	analyzers.push_back(new Health(this));
	analyzers.push_back(new Loot(this));
	analyzers.push_back(new Pray(this));
	analyzers.push_back(new Wand(this));
	analyzers.push_back(new Wish(this));
	analyzers.push_back(new Zorkmid(this));
}

/* destructor */
Saiph::~Saiph() {
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
		delete *a;
	delete world;
	delete connection;
	debugfile.close();
}

/* methods */
void Saiph::addItemToInventory(unsigned char key, const Item &item) {
	if (item.count <= 0)
		return;
	debugfile << ITEMTRACKER_DEBUG_NAME << "Adding " << item.count << " " << item.name << " to inventory slot " << key << endl;
	if (inventory.find(key) != inventory.end()) {
		/* existing item, add amount */
		inventory[key].count += item.count;
	} else {
		/* new item */
		inventory[key] = item;
	}
}

unsigned char Saiph::directLine(Point point, bool ignore_sinks) {
	/* is the target in a direct line from the player? */
	if (point.row < MAP_ROW_BEGIN || point.row > MAP_ROW_END || point.col < MAP_COL_BEGIN || point.col > MAP_COL_END) {
		/* outside map */
		return ILLEGAL_MOVE;
	} else if (point == position) {
		/* eh? don't do this */
		return REST;
	} else if (point.row == position.row) {
		/* aligned horizontally */
		if (point.col > position.col) {
			while (--point.col > position.col) {
				if (!directLineHelper(point, ignore_sinks))
					return ILLEGAL_MOVE;
			}
			return MOVE_E;
		} else {
			while (++point.col < position.col) {
				if (!directLineHelper(point, ignore_sinks))
					return ILLEGAL_MOVE;
			}
			return MOVE_W;
		}
	} else if (point.col == position.col) {
		/* aligned vertically */
		if (point.row > position.row) {
			while (--point.row > position.row) {
				if (!directLineHelper(point, ignore_sinks))
					return ILLEGAL_MOVE;
			}
			return MOVE_S;
		} else {
			while (++point.row < position.row) {
				if (!directLineHelper(point, ignore_sinks))
					return ILLEGAL_MOVE;
			}
			return MOVE_N;
		}
	} else if (abs(point.row - position.row) == abs(point.col - position.col)) {
		/* aligned diagonally */
		if (point.row > position.row) {
			if (point.col > position.col) {
				while (--point.row > position.row) {
					--point.col;
					if (!directLineHelper(point, ignore_sinks))
						return ILLEGAL_MOVE;
				}
				return MOVE_SE;
			} else {
				while (--point.row > position.row) {
					++point.col;
					if (!directLineHelper(point, ignore_sinks))
						return ILLEGAL_MOVE;
				}
				return MOVE_SW;
			}
		} else {
			if (point.col > position.col) {
				while (++point.row < position.row) {
					--point.col;
					if (!directLineHelper(point, ignore_sinks))
						return ILLEGAL_MOVE;
				}
				return MOVE_NE;
			} else {
				while (++point.row < position.row) {
					++point.col;
					if (!directLineHelper(point, ignore_sinks))
						return ILLEGAL_MOVE;
				}
				return MOVE_NW;
			}
		}
	}
	return ILLEGAL_MOVE;
}

void Saiph::farlook(const Point &target) {
	/* look at something, eg. monster */
	/* TODO: make this an analyzer */
	string command;
	command.push_back(';');
	Point cursor(world->player.row, world->player.col);
	while (cursor.row != target.row && cursor.col != target.col) {
		unsigned char move;
		if (cursor.row < target.row && cursor.col < target.col) {
			move = MOVE_SE;
			++cursor.row;
			++cursor.col;
		} else if (cursor.row < target.row && cursor.col > target.col) {
			move = MOVE_SW;
			++cursor.row;
			--cursor.col;
		} else if (cursor.row > target.row && cursor.col < target.col) {
			move = MOVE_NE;
			--cursor.row;
			++cursor.col;
		} else if (cursor.row > target.row && cursor.col > target.col) {
			move = MOVE_NW;
			--cursor.row;
			--cursor.col;
		} else if (cursor.row < target.row) {
			move = MOVE_S;
			++cursor.row;
		} else if (cursor.row > target.row) {
			move = MOVE_N;
			--cursor.row;
		} else if (cursor.col < target.col) {
			move = MOVE_E;
			++cursor.col;
		} else {
			move = MOVE_W;
			--cursor.col;
		}
		command.push_back(move);
	}
	command.push_back(',');
	world->executeCommand(command);
}

void Saiph::removeItemFromInventory(unsigned char key, const Item &item) {
	if (item.count <= 0)
		return;
	map<unsigned char, Item>::iterator i = inventory.find(key);
	if (i == inventory.end())
		return;
	debugfile << ITEMTRACKER_DEBUG_NAME << "Removing " << item.count << " " << item.name << " from inventory slot " << key << endl;
	if (i->second.count > item.count)
		i->second.count -= item.count; // we got more than we remove
	else
		inventory.erase(i); // removing all we got
}

bool Saiph::request(const Request &request) {
	/* request an action from any analyzer */
	debugfile << REQUEST_DEBUG_NAME << request.request << ", " << request.priority << ", " << request.value << ", " << request.data << ", (" << request.coordinate.level << ", " << request.coordinate.row << ", " << request.coordinate.col << ")" << endl;
	bool status = false;
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a) {
		if ((*a)->request(request) && !status)
			status = true;
	}
	return status;
}

bool Saiph::run() {
	/* clear pickup list */
	pickup.clear();
	/* and drop list */
	drop.clear();
	/* set on_ground to NULL */
	on_ground = NULL;

	/* check if we're engulfed */
	if (position.row > MAP_ROW_BEGIN && position.row < MAP_ROW_END && position.col > MAP_COL_BEGIN && position.col < MAP_COL_END && world->view[position.row - 1][position.col - 1] == '/' && world->view[position.row - 1][position.col + 1] == '\\' && world->view[position.row + 1][position.col - 1] == '\\' && world->view[position.row + 1][position.col + 1] == '/')
		engulfed = true;
	else
		engulfed = false;

	/* detect player position */
	if (!world->question && !world->menu && !engulfed)
		detectPosition();

	/* level message parsing */
	debugfile << MESSAGES_DEBUG_NAME << "'" << world->messages << "'" << endl;
	levels[position.level].parseMessages(world->messages);

	/* set the on_ground pointer if there's loot here */
	if (levels[position.level].stashes.find(position) != levels[position.level].stashes.end())
		on_ground = &levels[position.level].stashes[position];

	/* update level */
	if (!world->question && !world->menu && !engulfed) {
		/* update changed symbols */
		for (vector<Point>::iterator c = world->changes.begin(); c != world->changes.end(); ++c)
			levels[position.level].updateMapPoint(*c, (unsigned char) world->view[c->row][c->col], world->color[c->row][c->col]);
		/* update monsters */
		levels[position.level].updateMonsters();
		/* update pathmap */
		levels[position.level].updatePathMap();
	}
	/* print maps so we see what we're doing */
	dumpMaps();

	/* analyzer stuff comes here */
	Analyzer *best_analyzer = NULL;
	best_priority = ILLEGAL_PRIORITY;

	/* clear priority from analyzers */
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
		(*a)->priority = ILLEGAL_PRIORITY;

	/* let analyzers parse messages */
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a) {
		(*a)->parseMessages(world->messages);
		if ((*a)->priority > best_priority) {
			best_priority = (*a)->priority;
			best_analyzer = *a;
		}
	}

	/* call begin() in analyzers */
	if (!world->question && !world->menu) {
		for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a) {
			(*a)->begin();
			if ((*a)->priority > best_priority) {
				best_priority = (*a)->priority;
				best_analyzer = *a;
			}
		}
	}

	/* inspect the dungeon */
	if (!world->question && !world->menu) {
		for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
			for (vector<Point>::iterator c = world->changes.begin(); c != world->changes.end(); ++c) {
				(*a)->inspect(*c);
			if ((*a)->priority > best_priority) {
				best_priority = (*a)->priority;
				best_analyzer = *a;
			}
		}
	}

	/* call finish() in analyzers */
	if (!world->question && !world->menu) {
		for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a) {
			(*a)->finish();
			if ((*a)->priority > best_priority) {
				best_priority = (*a)->priority;
				best_analyzer = *a;
			}
		}
	}

	/* check if we got a command */
	if (world->question && best_analyzer == NULL) {
		debugfile << SAIPH_DEBUG_NAME << "Unhandled question: " << world->messages << endl;
		return false;
	} else if (world->menu && best_analyzer == NULL) {
		debugfile << SAIPH_DEBUG_NAME << "Unhandled menu: " << world->messages << endl;
		return false;
	} else if (best_analyzer == NULL) {
		debugfile << SAIPH_DEBUG_NAME << "I have no idea what to do... Searching" << endl;
		world->executeCommand("s");
		return true;
	}

	/* let an analyzer do its command */
	debugfile << COMMAND_DEBUG_NAME << "'" << best_analyzer->command << "' from analyzer " << best_analyzer->name << " with priority " << best_priority << endl;
	world->executeCommand(best_analyzer->command);
	best_analyzer->complete();
	return true;
}

unsigned char Saiph::shortestPath(unsigned char symbol, bool allow_illegal_last_move, int *moves) {
	/* returns next move in shortest path from player to nearest symbol.
	 * also sets amount of moves to the target */
	if (!Level::track_symbol[symbol])
		return ILLEGAL_MOVE;
	int least_moves = INT_MAX;
	unsigned char best_move = ILLEGAL_MOVE;
	int level = 0;
	for (vector<Level>::iterator l = levels.begin(); l != levels.end(); ++l) {
		++level;
		for (map<Point, int>::iterator p = l->symbols[symbol].begin(); p != l->symbols[symbol].end(); ++p) {
			unsigned char move = shortestPath(Coordinate(level, p->first), allow_illegal_last_move, moves);
			if (move == ILLEGAL_MOVE)
				continue;
			if (level != position.level)
				*moves += abs(level - position.level) * 100; // on another level, add 100 moves per level
			if (*moves >= least_moves)
				continue;
			least_moves = *moves;
			best_move = move;
		}
	}
	return best_move;
}

unsigned char Saiph::shortestPath(const Coordinate &target, bool allow_illegal_last_move, int *moves) {
	/* returns next move in shortest path from player to target.
	 * also sets amount of moves to the target */
	if (target.level < 0 || target.level >= (int) levels.size())
		return ILLEGAL_MOVE; // outside the map
	if (target.level == position.level) {
		/* target on same level */
		return levels[position.level].shortestPath(target, allow_illegal_last_move, moves);
	} else {
		/* pathing to another level */
		/* breadth first search */
		int levelqueue[levels.size()];
		Point up_or_down[levels.size()];
		levelqueue[0] = position.level;
		int pivot = 0;
		int queuesize = 1;
		while (pivot < queuesize) {
			/* upstairs */
			for (map<Point, int>::iterator s = levels[levelqueue[pivot]].symbols[STAIRS_UP].begin(); s != levels[levelqueue[pivot]].symbols[STAIRS_UP].end(); ++s) {
				if (s->second == -1) {
					/* don't know where these stairs lead */
				} else if (s->second == target.level) {
					/* these stairs leads to the right level */
					unsigned char move = levels[levelqueue[pivot]].shortestPath(up_or_down[pivot], allow_illegal_last_move, moves);
					if (move == REST)
						return MOVE_UP;
					return move;
				} else {
					/* add stairs to queue */
					if (pivot == 0)
						up_or_down[queuesize] = s->first;
					else
						up_or_down[queuesize] = up_or_down[pivot];
					levelqueue[queuesize++] = s->second;
				}
			}
			/* downstairs */
			for (map<Point, int>::iterator s = levels[levelqueue[pivot]].symbols[STAIRS_DOWN].begin(); s != levels[levelqueue[pivot]].symbols[STAIRS_DOWN].end(); ++s) {
				if (s->second == -1) {
					/* don't know where these stairs lead */
				} else if (s->second == target.level) {
					/* these stairs leads to the right level */
					unsigned char move = levels[levelqueue[pivot]].shortestPath(up_or_down[pivot], allow_illegal_last_move, moves);
					if (move == REST)
						return MOVE_DOWN;
					return move;
				} else {
					/* add stairs to queue */
					if (pivot == 0)
						up_or_down[queuesize] = s->first;
					else
						up_or_down[queuesize] = up_or_down[pivot];
					levelqueue[queuesize++] = s->second;
				}
			}
			++pivot;
		}
	}
	return ILLEGAL_MOVE;
}

unsigned char Saiph::shortestPath(const Point &target, bool allow_illegal_last_move, int *moves) {
	/* pathing on same level */
	return levels[position.level].shortestPath(target, allow_illegal_last_move, moves);
}

/* private methods */
void Saiph::detectPosition() {
	if (position.level == -1) {
		/* this happens when we start */
		position.row = world->player.row;
		position.col = world->player.col;
		position.level = levels.size();
		levels.push_back(Level(this, world->player.level));
		levelmap[world->player.level].push_back(position.level);
		return;
	}
	if ((int) levels.size() >= position.level - 1 && strcmp(world->player.level, levels[position.level].name.c_str()) == 0) {
		/* same level as last frame, just update row & col */
		position.row = world->player.row;
		position.col = world->player.col;
		return;
	}
	/* level has changed.
	 * we need to figure out if it's a new level or one we already know of */
	int found = -1;
	/* maybe we already know where these stairs lead? */
	if (levels[position.level].dungeonmap[position.row][position.col] == STAIRS_DOWN) {
		/* we did stand on stairs down, and if we don't know where they lead then
		 * the next line will still just set found to -1 */
		found = levels[position.level].symbols[STAIRS_DOWN][position];
	} else if (levels[position.level].dungeonmap[position.row][position.col] == STAIRS_UP) {
		/* we did stand on stairs up, and if we don't know where they lead then
		 * the next line will still just set found to -1 */
		found = levels[position.level].symbols[STAIRS_UP][position];
	}
	string level = world->player.level;
	if (found == -1) {
		/* we didn't know where the stairs would take us */
		for (vector<int>::iterator lm = levelmap[level].begin(); lm != levelmap[level].end(); ++lm) {
			/* check if level got vertical walls on same locations.
			 * since walls can disappear, we'll allow a 70% match */
			int total = 0;
			int matched = 0;
			for (map<Point, int>::iterator s = levels[*lm].symbols[VERTICAL_WALL].begin(); s != levels[*lm].symbols[VERTICAL_WALL].end(); ++s) {
				if (world->view[s->first.row][s->first.col] == VERTICAL_WALL)
					++matched;
				++total;
			}
			if (matched * 10 >= total * 7) {
				found = *lm;
				break;
			}
		}
	}
	if (found == -1) {
		/* new level */
		found = levels.size();
		levels.push_back(Level(this, level));
	}
	/* were we on stairs on last position? */
	if (levels[position.level].dungeonmap[position.row][position.col] == STAIRS_DOWN) {
		/* yes, we were on stairs down */
		levels[position.level].symbols[STAIRS_DOWN][position] = found;
	} else if (levels[position.level].dungeonmap[position.row][position.col] == STAIRS_UP) {
		/* yes, we were on stairs up */
		levels[position.level].symbols[STAIRS_UP][position] = found;
	}
	position.row = world->player.row;
	position.col = world->player.col;
	position.level = found;
	levelmap[level].push_back(found);
}

bool Saiph::directLineHelper(const Point &point, bool ignore_sinks) {
	if (!Level::passable[levels[position.level].dungeonmap[point.row][point.col]])
		return false;
	else if (!ignore_sinks && levels[position.level].dungeonmap[point.row][point.col] == SINK)
		return false;
	else if (levels[position.level].monstermap[point.row][point.col] != ILLEGAL_MONSTER && levels[position.level].monsters[point].visible)
		return false;
	return true;
}

void Saiph::dumpMaps() {
	/* monsters */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		cout << (unsigned char) 27 << "[" << r + 26 << ";2H";
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			if (r == world->player.row && c == world->player.col)
				cout << (unsigned char) 27 << "[35m@" << (unsigned char) 27 << "[m";
			else if (levels[position.level].monstermap[r][c] != ILLEGAL_MONSTER)
				cout << (unsigned char) (levels[position.level].monstermap[r][c]);
			else
				cout << (unsigned char) (levels[position.level].dungeonmap[r][c]);
		}
	}
	/* world map as the bot sees it */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		cout << (unsigned char) 27 << "[" << r + 1 << ";82H";
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			if (r == world->player.row && c == world->player.col)
				cout << (unsigned char) 27 << "[35m";
			cout << (unsigned char) (levels[position.level].dungeonmap[r][c]);
			if (r == world->player.row && c == world->player.col)
				cout << (unsigned char) 27 << "[m";
		}
	}
	/* path map */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		cout << (unsigned char) 27 << "[" << r + 26 << ";82H";
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			if (r == world->player.row && c == world->player.col)
				cout << (unsigned char) 27 << "[35m@" << (unsigned char) 27 << "[m";
			else if (levels[position.level].pathmap.nodes[r][c].move >= 'a' && levels[position.level].pathmap.nodes[r][c].move <= 'z')
				//cout << (unsigned char) levels[position.level].pathmap.nodes[r][c].move;
				cout << (char) (levels[position.level].pathmap.nodes[r][c].cost % 64 + 65);
			else
				cout << (unsigned char) (levels[position.level].dungeonmap[r][c]);
		}
	}
	/* return cursor back to where it was */
	cout << (unsigned char) 27 << "[" << world->cursor.row + 1 << ";" << world->cursor.col + 1 << "H";
	/* and flush cout. if we don't do this our output looks like garbage */
	cout.flush();
}

/* main */
int main() {
	Saiph *saiph = new Saiph(CONNECTION_TELNET);
	//for (int a = 0; a < 200 && saiph->run(); ++a)
	//	;
	while (saiph->run())
		;
	saiph->debugfile << SAIPH_DEBUG_NAME << "Quitting gracefully" << endl;
	delete saiph;
}
