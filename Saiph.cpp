#include "Saiph.h"

/* constructors */
Saiph::Saiph(bool remote) {
	connection = new Connection(remote);
	world = new World(connection);

	/* engulfed */
	engulfed = false;

	/* set certain values */
	for (int a = 0; a <= UCHAR_MAX; ++a) {
		/* monsters */
		if ((a >= '@' && a <= 'Z') || (a >= 'a' && a <= 'z') || (a >= '1' && a <= '6')  || a == '&' || a == '\'' || a == ':' || a == ';' || a == '~' || a == PET)
			monster[a] = true;
		else
			monster[a] = false;
		/* items */
		item[a] = false;
		/* pathing & maps */
		passable[a] = false;
		static_dungeon_symbol[a] = false;
		pathcost[a] = 0;
	}
	/* items */
	item[(unsigned char) WEAPON] = true;
	item[(unsigned char) ARMOR] = true;
	item[(unsigned char) RING] = true;
	item[(unsigned char) AMULET] = true;
	item[(unsigned char) TOOL] = true;
	item[(unsigned char) FOOD] = true;
	item[(unsigned char) POTION] = true;
	item[(unsigned char) SCROLL] = true;
	item[(unsigned char) SPELLBOOK] = true;
	item[(unsigned char) WAND] = true;
	item[(unsigned char) GOLD] = true;
	item[(unsigned char) GEM] = true;
	item[(unsigned char) STATUE] = true;
	item[(unsigned char) BOULDER] = true;
	item[(unsigned char) IRON_BALL] = true;
	item[(unsigned char) CHAINS] = true;
	item[(unsigned char) VENOM] = true;
	/* pathing & maps */
	passable[(unsigned char) FLOOR] = true;
	passable[(unsigned char) OPEN_DOOR] = true;
	passable[(unsigned char) CORRIDOR] = true;
	passable[(unsigned char) STAIRS_UP] = true;
	passable[(unsigned char) STAIRS_DOWN] = true;
	passable[(unsigned char) ALTAR] = true;
	passable[(unsigned char) GRAVE] = true;
	passable[(unsigned char) THRONE] = true;
	passable[(unsigned char) SINK] = true;
	passable[(unsigned char) FOUNTAIN] = true;
	passable[(unsigned char) WATER] = true;
	passable[(unsigned char) ICE] = true;
	passable[(unsigned char) LAVA] = true;
	passable[(unsigned char) LOWERED_DRAWBRIDGE] = true;
	passable[(unsigned char) TRAP] = true;
	passable[(unsigned char) WEAPON] = true;
	passable[(unsigned char) ARMOR] = true;
	passable[(unsigned char) RING] = true;
	passable[(unsigned char) AMULET] = true;
	passable[(unsigned char) TOOL] = true;
	passable[(unsigned char) FOOD] = true;
	passable[(unsigned char) POTION] = true;
	passable[(unsigned char) SCROLL] = true;
	passable[(unsigned char) SPELLBOOK] = true;
	passable[(unsigned char) WAND] = true;
	passable[(unsigned char) GOLD] = true;
	passable[(unsigned char) GEM] = true;
	passable[(unsigned char) STATUE] = true;
	passable[(unsigned char) IRON_BALL] = true;
	passable[(unsigned char) CHAINS] = true;
	passable[(unsigned char) VENOM] = true;
	passable[(unsigned char) PET] = true;
	static_dungeon_symbol[(unsigned char) VERTICAL_WALL] = true;
	static_dungeon_symbol[(unsigned char) HORIZONTAL_WALL] = true;
	static_dungeon_symbol[(unsigned char) FLOOR] = true;
	static_dungeon_symbol[(unsigned char) OPEN_DOOR] = true;
	static_dungeon_symbol[(unsigned char) CLOSED_DOOR] = true;
	static_dungeon_symbol[(unsigned char) IRON_BARS] = true;
	static_dungeon_symbol[(unsigned char) TREE] = true;
	static_dungeon_symbol[(unsigned char) CORRIDOR] = true;
	static_dungeon_symbol[(unsigned char) STAIRS_UP] = true;
	static_dungeon_symbol[(unsigned char) STAIRS_DOWN] = true;
	static_dungeon_symbol[(unsigned char) ALTAR] = true;
	static_dungeon_symbol[(unsigned char) GRAVE] = true;
	static_dungeon_symbol[(unsigned char) THRONE] = true;
	static_dungeon_symbol[(unsigned char) SINK] = true;
	static_dungeon_symbol[(unsigned char) FOUNTAIN] = true;
	static_dungeon_symbol[(unsigned char) WATER] = true;
	static_dungeon_symbol[(unsigned char) ICE] = true;
	static_dungeon_symbol[(unsigned char) LAVA] = true;
	static_dungeon_symbol[(unsigned char) LOWERED_DRAWBRIDGE] = true;
	static_dungeon_symbol[(unsigned char) RAISED_DRAWBRIDGE] = true;
	static_dungeon_symbol[(unsigned char) TRAP] = true;
	static_dungeon_symbol[(unsigned char) BOULDER] = true; // hardly static, but we won't allow moving on to one
	pathcost[ICE] = COST_ICE;
	pathcost[LAVA] = COST_LAVA;
	pathcost[PET] = COST_PET;
	pathcost[TRAP] = COST_TRAP;
	pathcost[WATER] = COST_WATER;

	/* messages */
	messages.clear();

	/* Analyzers */
	analyzers.push_back(new DoorAnalyzer(this));
	analyzers.push_back(new FoodAnalyzer(this));
	analyzers.push_back(new ExploreAnalyzer(this));
	analyzers.push_back(new HealthAnalyzer(this));
	analyzers.push_back(new LevelAnalyzer(this));
	analyzers.push_back(new LootAnalyzer(this));
	analyzers.push_back(new MonsterAnalyzer(this));
}

/* destructors */
Saiph::~Saiph() {
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
		delete *a;
	delete world;
	delete connection;
}

/* methods */
void Saiph::farlook(const Point &target) {
	/* look at something, eg. monster */
	command.push_back(';');
	Point cursor;
	cursor.row = world->player.row;
	cursor.col = world->player.col;
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
	cerr << command << endl;
	world->executeCommand(command);
}

void Saiph::registerAnalyzerSymbols(Analyzer *analyzer, const vector<unsigned char> &symbols) {
	/* register which symbols analyzers should analyze */
	for (vector<unsigned char>::const_iterator s = symbols.begin(); s != symbols.end(); ++s)
		analyzer_symbols[*s].push_back(analyzer);
}

bool Saiph::run() {
	/* figure out which map to use, FIXME */
	current_branch = 0;
	current_level = world->player.dungeon;

	/* update maps */
	if (!world->question && !world->menu)
		updateMaps();

	/* print stuff so we see what we're doing */
	dumpMaps();

	/* reset command */
	int best_analyzer = -1;
	int best_priority = 0;

	/* check if we're engulfed */
	int r = world->player.row;
	int c = world->player.col;
	if (r > MAP_ROW_BEGIN && r < MAP_ROW_END && c > MAP_COL_BEGIN && c < MAP_COL_END && world->map[r - 1][c] == '-' && world->map[r + 1][c] == '-' && world->map[r][c - 1] == '|' && world->map[r][c + 1] == '|')
		engulfed = true;
	else
		engulfed = false;

	/* deal with messages */
	/* and again, we could make a map<string, Analyzer *> to speed things up.
	 * the first string being the message.
	 * then again, what about "you see here a %s corpse"? */
	messages = world->messages;
	cerr << messages << endl;
	for (vector<Analyzer>::size_type a = 0; a < analyzers.size(); ++a) {
		int priority = analyzers[a]->parseMessages(&messages);
		if (priority > best_priority) {
			best_analyzer = a;
			best_priority = priority;
		}
	}

	/* call start() in analyzers */
	if (!world->question && !world->menu) {
		for (vector<Analyzer>::size_type a = 0; a < analyzers.size(); ++a) {
			int priority = analyzers[a]->start();
			if (priority > best_priority) {
				best_analyzer = a;
				best_priority = priority;
			}
		}
	}

	/* inspect the dungeon */
	if (!world->question && !world->menu)
		inspect();

	/* call finish() in analyzers */
	if (!world->question && !world->menu) {
		for (vector<Analyzer>::size_type a = 0; a < analyzers.size(); ++a) {
			int priority = analyzers[a]->finish();
			if (priority > best_priority) {
				best_analyzer = a;
				best_priority = priority;
			}
		}
	}

	if (world->question && best_analyzer == -1) {
		cerr << "Unhandled question: " << messages << endl;
		return false;
	}

	if (world->menu && best_analyzer == -1) {
		cerr << "Unhandled menu: " << messages << endl;
		return false;
	}

	cerr << "letting analyzer " << best_analyzer << " do its thing with priority " << best_priority << endl;

	/* check if we got a command */
	if (best_analyzer == -1)
		return false;

	/* let an analyzer do its command */
	command.clear(); // just in case some analyzer messed with this string
	analyzers[best_analyzer]->command(&command);
	world->executeCommand(command);
	return true;
}

unsigned char Saiph::shortestPath(const Point &target, bool allow_illegal_last_move, int *distance, bool *straight_line) {
	/* returns next move in shortest path from player to target.
	 * also sets "distance" and "straight_line" to target */
	if (target.row < MAP_ROW_BEGIN || target.row > MAP_ROW_END || target.col < MAP_COL_BEGIN || target.col > MAP_COL_END)
		return ILLEGAL_MOVE; // outside the map
	PathNode *node = &pathmap[target.row][target.col];
	*distance = 0;
	*straight_line = true;
	if (node->cost == 0)
		return REST; // pathing to player?
	++*distance;
	unsigned char move = ILLEGAL_MOVE;
	unsigned char previous_move = ILLEGAL_MOVE; // used to determine straight_line
	if (allow_illegal_last_move && node->nextnode == NULL) {
		/* sometimes we wish to move somewhere we really can't move to.
		 * for example: fighting a monster in a wall or through "corner".
		 * solution: find adjacent squares with lowest cost and backtrack from there */
		/* note:
		 * since we're moving from target to player,
		 * we're moving the opposite direction of the node we're checking */
		/* northwest node */
		int row = target.row - 1;
		int col = target.col - 1;
		move = MOVE_SE;
		node = &pathmap[row][col];
		unsigned int lowest_cost = node->cost;
		/* north node */
		++col;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_S;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* northeast node */
		++col;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_SW;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* east node */
		++row;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_W;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* southeast node */
		++row;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_NW;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* south node */
		--col;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_N;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* southwest node */
		--col;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_NE;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* west node */
		--row;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_E;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		previous_move = move;
		if (lowest_cost == 0)
			return move; // found the player
		++*distance;
	}
	if (node->nextnode == NULL)
		return ILLEGAL_MOVE; // couldn't find path

	while (node->nextnode != NULL) {
		previous_move = move;
		move = node->move;
		if (*distance > 1 && previous_move != move)
			*straight_line = false;
		++*distance;
		node = node->nextnode;
	}
	return move;
}

/* private methods */
void Saiph::dumpMaps() {
	/* search map */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		cout << (unsigned char) 27 << "[" << r + 26 << ";2H";
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			cout << (unsigned char) (map[current_branch][current_level].search[r][c] % 96 + 32);
		}
	}
	/* world map as the bot sees it */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		cout << (unsigned char) 27 << "[" << r + 1 << ";82H";
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			cout << (unsigned char) (map[current_branch][current_level].dungeon[r][c]);
		}
	}
	/* path map */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		cout << (unsigned char) 27 << "[" << r + 26 << ";82H";
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			//cout << (unsigned char) (pathmap[r][c].cost % 96 + 32);
			if (r == world->player.row && c == world->player.col)
				cout << '@';
			else if (pathmap[r][c].move >= 'a' && pathmap[r][c].move <= 'z')
				cout << (unsigned char) pathmap[r][c].move;
			else
				cout << ' ';
		}
	}
	/* return cursor back to where it was */
	cout << (unsigned char) 27 << "[" << world->row + 1 << ";" << world->col + 1 << "H";
}

void Saiph::inspect() {
	/* inspect the dungeon for interesting monsters/objects/places */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			unsigned char symbol = world->map[r][c];
			if (symbol == SOLID_ROCK) // unlit rooms makes floor go back to SOLID_ROCK
				symbol = map[current_branch][current_level].dungeon[r][c];
			for (vector<Analyzer *>::iterator a = analyzer_symbols[symbol].begin(); a != analyzer_symbols[symbol].end(); ++a)
				(*a)->analyze(r, c, symbol);
		}
	}
}

void Saiph::updateMaps() {
	/* update the various maps */
	/* this loop is a small hack. FIXME: make own monster tracker that does this
	 * it removes unseen monsters next to the player which we think are blocking the path */
	for (int r = world->player.row - 1; r <= world->player.row + 1; ++r) {
		if (r < MAP_ROW_BEGIN || r > MAP_ROW_END)
			continue;
		for (int c = world->player.col - 1; c <= world->player.col + 1; ++c) {
			if (c < MAP_COL_BEGIN || c > MAP_COL_END)
				continue;
			if (map[current_branch][current_level].monster[r][c] != NOMONSTER)
				map[current_branch][current_level].monster[r][c] = NOMONSTER;
		}
	}
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			unsigned char s = world->map[r][c];
			if (s == SOLID_ROCK)
				continue; // not interesting (also mess up unlit rooms)
			/* "static" dungean features */
			if (static_dungeon_symbol[s]) {
				/* update the map showing static stuff */
				map[current_branch][current_level].dungeon[r][c] = s;
			} else if (!passable[map[current_branch][current_level].dungeon[r][c]]) {
				/* hmm, this tile used to be unpassable,
				 * but now it isn't...
				 * this happens for example when:
				 * - monster opens door and we kill the monster in the doorway
				 * - monster digging out level, leaving stuff on squares
				 * let's place an open door here */
				map[current_branch][current_level].dungeon[r][c] = OPEN_DOOR;
			}
			if (item[s]) {
				/* item here */
				map[current_branch][current_level].item[r][c] = s;
			} else if (map[current_branch][current_level].item[r][c] != NOITEM) {
				/* item is gone? hmm, remove it */
				map[current_branch][current_level].item[r][c] = NOITEM;
			}
			if (monster[s]) {
				/* found a monster!
				 * since monsters unlike items disappear from map when we can't see them,
				 * we can't remove monsters like we do with items above.
				 * we'll have some dedicated code for fixing this */
				map[current_branch][current_level].monster[r][c] = s;
			}
		}
	}

	updatePathMap();
}

void Saiph::updatePathMap() {
	/* first reset nextnode pointer, cost & move */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			pathmap[r][c].nextnode = NULL;
			pathmap[r][c].cost = UINT_MAX;
			pathmap[r][c].move = ILLEGAL_MOVE;
		}
	}
	Point from;
	from.row = world->player.row;
	from.col = world->player.col;
	pathing_queue[0] = from;
	pathmap[from.row][from.col].cost = 0;
	int curnode = 0;
	int nodes = 1;
	while (curnode < nodes) {
		from = pathing_queue[curnode++];
		if (from.row <= MAP_ROW_BEGIN || from.row >= MAP_ROW_END || from.col <= MAP_COL_BEGIN || from.col >= MAP_COL_END)
			continue; // too close to the edge
		Point to;
		/* check northwest node */
		to.row = from.row - 1;
		to.col = from.col - 1;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_NW;
			pathing_queue[nodes++] = to;
		}
		/* check north node */
		++to.col;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_N;
			pathing_queue[nodes++] = to;
		}
		/* check northeast node */
		++to.col;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_NE;
			pathing_queue[nodes++] = to;
		}
		/* check east node */
		++to.row;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_E;
			pathing_queue[nodes++] = to;
		}
		/* check southeast node */
		++to.row;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_SE;
			pathing_queue[nodes++] = to;
		}
		/* check south node */
		--to.col;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_S;
			pathing_queue[nodes++] = to;
		}
		/* check southwest node */
		--to.col;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_SW;
			pathing_queue[nodes++] = to;
		}
		/* check west node */
		--to.row;
		if (updatePathMapHelper(to, from)) {
			pathmap[to.row][to.col].move = MOVE_W;
			pathing_queue[nodes++] = to;
		}
	}
}

bool Saiph::updatePathMapHelper(const Point &to, const Point &from) {
	/* helper method for updatePathMap()
	 * return true if the move is legal and we should path further from this node */
	unsigned char s = map[current_branch][current_level].dungeon[to.row][to.col];
	if (!passable[s])
		return false;
	unsigned char m = map[current_branch][current_level].monster[to.row][to.col];
	if (monster[m] && m != PET)
		return false; // can't path through monsters (except pets)
	bool cardinal_move = (to.row == from.row || to.col == from.col);
	if (!cardinal_move) {
		if (s == OPEN_DOOR || map[current_branch][current_level].dungeon[from.row][from.col] == OPEN_DOOR)
			return false; // diagonally in/out of door
		unsigned char sc1 = map[current_branch][current_level].dungeon[to.row][from.col];
		unsigned char sc2 = map[current_branch][current_level].dungeon[from.row][to.col];
		if (!passable[sc1] && !passable[sc2]) {
			/* moving past two corners
			 * while we may pass two corners if we're not carrying too much we'll just ignore this.
			 * it's bound to cause issues */
			if (sc1 != BOULDER && sc2 != BOULDER)
				return false; // neither corner is a boulder, we may not pass
			//else if (in_sokoban)
			//	return false;
		}
	}
	//if (blacklisted_move)
	//	return false;
	//if (polymorphed_to_grid_bug)
	//	return false;
	//if (s == LAVA && !levitating)
	//	return false;
	//if (s == WATER && (!levitating || !waterwalk))
	//	return false;
	unsigned int newcost = pathmap[from.row][from.col].cost + (cardinal_move ? COST_CARDINAL : COST_DIAGONAL);
	newcost += pathcost[s];
	newcost += pathcost[m];
	if (newcost < pathmap[to.row][to.col].cost) {
		pathmap[to.row][to.col].nextnode = &pathmap[from.row][from.col];
		pathmap[to.row][to.col].cost = newcost;
		return true;
	}
	return false;
}

/* main */
int main() {
	Saiph *saiph = new Saiph(false);
	//for (int a = 0; a < 200 && saiph->run(); ++a)
	//	;
	while (saiph->run())
		;
	delete saiph;
}
