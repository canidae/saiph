#include "Saiph.h"

/* constructors */
Saiph::Saiph(int interface) {
	connection = Connection::create(interface);
	if (connection == NULL) {
		cout << "ERROR: Don't know what interface this is: " << interface << endl;
		exit(1);
	}
	world = new World(connection);
	monstertracker = new MonsterTracker(this);

	/* engulfed */
	engulfed = false;

	/* set certain values */
	for (int a = 0; a <= UCHAR_MAX; ++a) {
		/* monsters */
		if ((a >= '@' && a <= 'Z') || (a >= 'a' && a <= 'z') || (a >= '1' && a <= '5')  || a == '&' || a == '\'' || a == ':' || a == ';' || a == '~' || a == PET)
			monster[a] = true;
		else
			monster[a] = false;
		/* pathing & maps */
		passable[a] = false;
		static_dungeon_symbol[a] = false;
		pathcost[a] = 0;
	}
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
	passable[(unsigned char) UNKNOWN_TILE] = true;
	passable[(unsigned char) UNKNOWN_TILE_DIAGONALLY_PASSABLE] = true;
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

	/* Analyzers */
	analyzers.push_back(new Door(this));
	analyzers.push_back(new Elbereth(this));
	analyzers.push_back(new Explore(this));
	analyzers.push_back(new Fight(this));
	analyzers.push_back(new Food(this));
	analyzers.push_back(new Health(this));
	analyzers.push_back(new Level(this));
	analyzers.push_back(new Loot(this));
}

/* destructor */
Saiph::~Saiph() {
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
		delete *a;
	delete monstertracker;
	delete world;
	delete connection;
}

/* methods */
void Saiph::announce(const Announce &announce) {
	/* announce something to other analyzers */
	cerr << "[ANNOUNCE] " << announce.announce << ", " << announce.priority << ", " << announce.value1 << ", " << announce.data << ", (" << announce.coordinate.branch << ", " << announce.coordinate.level << ", " << announce.coordinate.row << ", " << announce.coordinate.col << ")" << endl;
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
		(*a)->announce(announce);
}

void Saiph::farlook(const Point &target) {
	/* look at something, eg. monster */
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

bool Saiph::request(const Request &request) {
	/* request an action from any analyzer */
	cerr << "[REQUEST ] " << request.request << ", " << request.priority << ", " << request.value1 << ", " << request.data << ", (" << request.coordinate.branch << ", " << request.coordinate.level << ", " << request.coordinate.row << ", " << request.coordinate.col << ")" << endl;
	bool status = false;
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a) {
		if ((*a)->request(request) && !status)
			status = true;
	}
	return status;
}

bool Saiph::run() {
	/* figure out which map to use.
	 * TODO: we need some branch detection & stuff here */
	current_branch = 0;
	current_level = world->player.dungeon;

	/* check if we're engulfed */
	int r = world->player.row;
	int c = world->player.col;
	if (r > MAP_ROW_BEGIN && r < MAP_ROW_END && c > MAP_COL_BEGIN && c < MAP_COL_END && world->view[r - 1][c - 1] == '/' && world->view[r - 1][c + 1] == '\\' && world->view[r + 1][c - 1] == '\\' && world->view[r + 1][c + 1] == '/')
		engulfed = true;
	else
		engulfed = false;

	/* update maps */
	if (!world->question && !world->menu && !engulfed)
		updateMaps();

	/* print stuff so we see what we're doing */
	dumpMaps();

	/* clear analyzers priority */
	for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
		(*a)->priority = ILLEGAL_PRIORITY;

	/* deal with messages */
	cerr << "[MESSAGES] '" << world->messages << "'" << endl;
	/* global parsing */
	parseMessages();
	/* then analyzer parsing */
	for (vector<Analyzer *>::size_type a = 0; a < analyzers.size(); ++a)
		analyzers[a]->parseMessages(&world->messages);

	/* call start() in analyzers */
	if (!world->question && !world->menu) {
		for (vector<Analyzer *>::size_type a = 0; a < analyzers.size(); ++a)
			analyzers[a]->start();
	}

	/* inspect the dungeon */
	if (!world->question && !world->menu)
		inspect();

	/* call finish() in analyzers */
	if (!world->question && !world->menu) {
		for (vector<Analyzer *>::size_type a = 0; a < analyzers.size(); ++a)
			analyzers[a]->finish();
	}

	/* check if we got a command */
	int best_analyzer = -1;
	int best_priority = ILLEGAL_PRIORITY;
	for (vector<Analyzer *>::size_type a = 0; a < analyzers.size(); ++a) {
		if (analyzers[a]->priority > best_priority) {
			best_priority = analyzers[a]->priority;
			best_analyzer = a;
		}
	}

	if (world->question && best_analyzer == -1) {
		cerr << "Unhandled question: " << world->messages << endl;
		return false;
	}

	if (world->menu && best_analyzer == -1) {
		cerr << "Unhandled menu: " << world->messages << endl;
		return false;
	}

	if (best_analyzer == -1)
		return false;

	/* let an analyzer do its command */
	command.clear(); // just in case some analyzer messed with this string
	analyzers[best_analyzer]->command(&command);
	cerr << "[COMMAND ] '" << command << "' from analyzer " << analyzers[best_analyzer]->name << " with priority " << best_priority << endl;
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
		/* also, to make her move more cardinally in corridors check cardinal moves first */

		/* north node */
		int row = target.row - 1;
		int col = target.col;
		node = &pathmap[row][col];
		unsigned int lowest_cost = node->cost;
		move = MOVE_S;
		/* east node */
		++row;
		++col;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_W;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* south node */
		++row;
		--col;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_N;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* west node */
		--row;
		--col;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_E;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* northwest node */
		--row;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_SE;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* northeast node */
		col += 2;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_SW;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* southeast node */
		row += 2;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_NW;
			node = &pathmap[row][col];
			lowest_cost = node->cost;
		}
		/* southwest node */
		col -= 2;
		if (pathmap[row][col].cost < lowest_cost) {
			move = MOVE_NE;
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
	/* monsters */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		cout << (unsigned char) 27 << "[" << r + 26 << ";2H";
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			if (r == world->player.row && c == world->player.col)
				cout << (unsigned char) 27 << "[35m@" << (unsigned char) 27 << "[m";
			else if (map[current_branch][current_level].monster[r][c] != ILLEGAL_MONSTER)
				cout << (unsigned char) (map[current_branch][current_level].monster[r][c]);
			else
				cout << (unsigned char) (map[current_branch][current_level].dungeon[r][c]);
		}
	}
	/* world map as the bot sees it */
	for (int r = MAP_ROW_BEGIN; r <= MAP_ROW_END; ++r) {
		cout << (unsigned char) 27 << "[" << r + 1 << ";82H";
		for (int c = MAP_COL_BEGIN; c <= MAP_COL_END; ++c) {
			if (r == world->player.row && c == world->player.col)
				cout << (unsigned char) 27 << "[35m";
			cout << (unsigned char) (map[current_branch][current_level].dungeon[r][c]);
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
			else if (pathmap[r][c].move >= 'a' && pathmap[r][c].move <= 'z')
				cout << (unsigned char) pathmap[r][c].move;
			else
				cout << (unsigned char) (map[current_branch][current_level].dungeon[r][c]);
		}
	}
	/* return cursor back to where it was */
	cout << (unsigned char) 27 << "[" << world->cursor.row + 1 << ";" << world->cursor.col + 1 << "H";
}

void Saiph::inspect() {
	/* notify the analyzers about changes */
	for (vector<Point>::iterator c = world->changes.begin(); c != world->changes.end(); ++c) {
		for (vector<Analyzer *>::iterator a = analyzers.begin(); a != analyzers.end(); ++a)
			(*a)->inspect(*c);
	}
}

void Saiph::parseMessages() {
	/* parse messages that can help us find doors/staircases/etc. */
	if (world->messages.find(MESSAGE_STAIRCASE_UP, 0) != string::npos)
		map[current_branch][current_level].dungeon[world->player.row][world->player.col] = STAIRS_UP;
	else if (world->messages.find(MESSAGE_STAIRCASE_DOWN, 0) != string::npos)
		map[current_branch][current_level].dungeon[world->player.row][world->player.col] = STAIRS_DOWN;
	else if (world->messages.find(MESSAGE_OPEN_DOOR, 0) != string::npos)
		map[current_branch][current_level].dungeon[world->player.row][world->player.col] = OPEN_DOOR;

	/* finally:
	 * if player is standing on an UNKNOWN_TILE and we didn't get "there's an open door here",
	 * then it's highly likely we can move diagonally to/from this point */
	if (map[current_branch][current_level].dungeon[world->player.row][world->player.col] == UNKNOWN_TILE)
		map[current_branch][current_level].dungeon[world->player.row][world->player.col] = UNKNOWN_TILE_DIAGONALLY_PASSABLE;
}

void Saiph::updateMaps() {
	/* update the various maps */
	for (vector<Point>::iterator c = world->changes.begin(); c != world->changes.end(); ++c) {
		unsigned char s = world->view[c->row][c->col];
		if (s == SOLID_ROCK)
			continue; // not interesting (also mess up unlit rooms)
		if (static_dungeon_symbol[s]) {
			/* update the map showing static stuff */
			map[current_branch][current_level].dungeon[c->row][c->col] = s;
		} else if (!passable[map[current_branch][current_level].dungeon[c->row][c->col]]) {
			/* we can't see the floor here, but we believe we can pass this tile.
			 * place an UNKNOWN_TILE here.
			 * the reason we check if stored tile is !passable is because if we don't,
			 * then every tile a monster steps on or drops an item on will become UNKNOWN_TILE,
			 * even if we already know what's beneath the monster/item. */
			map[current_branch][current_level].dungeon[c->row][c->col] = UNKNOWN_TILE;
		}
		if (monster[s]) {
			/* found a monster!
			 * since monsters unlike items disappear from map when we can't see them,
			 * we can't remove monsters like we do with items above.
			 * that's what MonsterTracker is for */
			monstertracker->updateMonster(*c);
		}
	}
	/* remove monsters that seems to be gone */
	monstertracker->removeMonsters();
	/* update map used for pathing */
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
	Point from(world->player.row, world->player.col);
	pathing_queue[0] = from;
	pathmap[from.row][from.col].cost = 0;
	pathmap[from.row][from.col].move = REST;
	int curnode = 0;
	int nodes = 1;
	while (curnode < nodes) {
		from = pathing_queue[curnode++];
		/* check northwest node */
		Point to(from.row - 1, from.col - 1);
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
	if (to.row < MAP_ROW_BEGIN || to.row > MAP_ROW_END || to.col < MAP_COL_BEGIN || to.col > MAP_COL_END)
		return false; // outside map
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
		if (s == UNKNOWN_TILE || map[current_branch][current_level].dungeon[from.row][from.col] == UNKNOWN_TILE)
			return false; // don't know what tile this is, it may be a door. no diagonal movement
		unsigned char sc1 = map[current_branch][current_level].dungeon[to.row][from.col];
		unsigned char sc2 = map[current_branch][current_level].dungeon[from.row][to.col];
		if (!passable[sc1] && !passable[sc2]) {
			/* moving past two corners
			 * while we may pass two corners if we're not carrying too much we'll just ignore this.
			 * it's bound to cause issues */
			if (sc1 != BOULDER && sc2 != BOULDER)
				return false; // neither corner is a boulder, we may not pass
			else if (current_branch == BRANCH_SOKOBAN)
				return false; // in sokoban we can't pass by boulders diagonally
		}
		//if (polymorphed_to_grid_bug)
		//	return false;
	}
	//if (blacklisted_move)
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
	Saiph *saiph = new Saiph(CONNECTION_TELNET);
	//for (int a = 0; a < 200 && saiph->run(); ++a)
	//	;
	while (saiph->run())
		;
	delete saiph;
}
