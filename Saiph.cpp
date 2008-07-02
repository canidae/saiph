#include "Saiph.h"

/* constructors */
Saiph::Saiph(bool remote) {
	connection = new Connection(remote);
	world = new World(connection);

	/* next command */
	command.analyzer = -1;
	command.priority = 0;

	/* engulfed */
	engulfed = false;

	/* set certain values */
	for (int a = 0; a <= UCHAR_MAX; ++a) {
		/* monsters */
		if ((a >= '@' && a <= 'Z') || (a >= 'a' && a <= 'z') || a == '&' || a == '\'' || a == '6' || a == ':' || a == ';' || a == '~')
			monster[a] = true;
		else
			monster[a] = false;
		/* items */
		item[a] = false;
		/* pathing & maps */
		passable[a] = false;
		static_dungeon_symbol[a] = false;
	}
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

	/* messages */
	messages.clear();

	/* Analyzers */
	analyzers.push_back(new DoorAnalyzer(this));
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
	world->command.push_back(';');
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
		world->command.push_back(move);
	}
	world->command.push_back(',');
	cerr << world->command << endl;
	world->executeCommand();
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
	command.analyzer = -1;
	command.priority = 0;

	/* check if we're engulfed */
	int r = world->player.row;
	int c = world->player.col;
	if (r > MAP_ROW_START && r < MAP_ROW_END && c > 0 && c < COLS && world->map[r - 1][c] == '-' && world->map[r + 1][c] == '-' && world->map[r][c - 1] == '|' && world->map[r][c + 1] == '|')
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
		if (priority > command.priority) {
			command.analyzer = a;
			command.priority = priority;
		}
	}

	/* call start() in analyzers */
	if (!world->question && !world->menu) {
		for (vector<Analyzer>::size_type a = 0; a < analyzers.size(); ++a) {
			int priority = analyzers[a]->start();
			if (priority > command.priority) {
				command.analyzer = a;
				command.priority = priority;
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
			if (priority > command.priority) {
				command.analyzer = a;
				command.priority = priority;
			}
		}
	}

	if (world->question && command.analyzer == -1) {
		cerr << "Unhandled question: " << messages << endl;
		return false;
	}

	if (world->menu && command.analyzer == -1) {
		cerr << "Unhandled menu: " << messages << endl;
		return false;
	}

	cerr << "letting analyzer " << command.analyzer << " do its thing with priority " << command.priority << endl;

	/* check if we got a command */
	if (command.analyzer == -1)
		return false;

	/* let an analyzer do its command */
	world->command.clear(); // just in case some analyzer messed with this string
	analyzers[command.analyzer]->command(&world->command);
	world->executeCommand();
	return true;
}

unsigned char Saiph::shortestPath(const Point &target, bool allow_illegal_last_move, int *distance, bool *straight_line) {
	/* returns next move in shortest path from player to target.
	 * also sets "distance" and "straight_line" to target */
	if (target.row <= MAP_ROW_START || target.row >= MAP_ROW_END || target.col < 1 || target.col > COLS - 1)
		return ILLEGAL_MOVE; // this means we can't move to the edges of the map (hopefully "ok")
	unsigned int curcost = pathmap[target.row][target.col];
	if (curcost == UINT_MAX && !allow_illegal_last_move)
		return ILLEGAL_MOVE; // can't move here
	if (curcost == 0)
		return REST; // we're standing on the pos we're looking for?
	*distance = 0;
	*straight_line = true;

	unsigned char move = ILLEGAL_MOVE;
	unsigned char previous_move = ILLEGAL_MOVE; // used to determine straight_line
	Point from = target;

	while (curcost > 0) {
		Point next = from;
		Point to = from;
		--to.row;
		--to.col;
		if (pathmap[to.row][to.col] < curcost && isLegalMove(to, from)) {
			move = MOVE_SE;
			curcost = pathmap[to.row][to.col];
			next = to;
		}
		++to.col;
		if (pathmap[to.row][to.col] < curcost && isLegalMove(to, from)) {
			move = MOVE_S;
			curcost = pathmap[to.row][to.col];
			next = to;
		}
		++to.col;
		if (pathmap[to.row][to.col] < curcost && isLegalMove(to, from)) {
			move = MOVE_SW;
			curcost = pathmap[to.row][to.col];
			next = to;
		}
		++to.row;
		if (pathmap[to.row][to.col] < curcost && isLegalMove(to, from)) {
			move = MOVE_W;
			curcost = pathmap[to.row][to.col];
			next = to;
		}
		++to.row;
		if (pathmap[to.row][to.col] < curcost && isLegalMove(to, from)) {
			move = MOVE_NW;
			curcost = pathmap[to.row][to.col];
			next = to;
		}
		--to.col;
		if (pathmap[to.row][to.col] < curcost && isLegalMove(to, from)) {
			move = MOVE_N;
			curcost = pathmap[to.row][to.col];
			next = to;
		}
		--to.col;
		if (pathmap[to.row][to.col] < curcost && isLegalMove(to, from)) {
			move = MOVE_NE;
			curcost = pathmap[to.row][to.col];
			next = to;
		}
		--to.row;
		if (pathmap[to.row][to.col] < curcost && isLegalMove(to, from)) {
			move = MOVE_E;
			curcost = pathmap[to.row][to.col];
			next = to;
		}
		if (from.row == next.row && from.col == next.col)
			return ILLEGAL_MOVE;
		from = next;
		if (*distance > 0 && previous_move != move)
			*straight_line = false;
		previous_move = move;
		++*distance;
		allow_illegal_last_move = false;
	}
	return move;
}

/* private methods */
void Saiph::dumpMaps() {
	/* search map */
	cout << (unsigned char) 27 << "[26;1H";
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c) {
			cout << (unsigned char) (map[current_branch][current_level].search[r][c] % 96 + 32);
		}
		cout << endl;
	}
	/* world map as the bot sees it */
	for (int r = 0; r < ROWS; ++r) {
		cout << (unsigned char) 27 << "[" << r + 1 << ";82H";
		for (int c = 0; c < COLS; ++c) {
			cout << (unsigned char) (map[current_branch][current_level].dungeon[r][c]);
		}
	}
	/* path map */
	for (int r = 0; r < ROWS; ++r) {
		cout << (unsigned char) 27 << "[" << r + 26 << ";82H";
		for (int c = 0; c < COLS; ++c) {
			cout << (unsigned char) (pathmap[r][c] % 96 + 32);
		}
	}
	/* return cursor back to player */
	cout << (unsigned char) 27 << "[" << world->player.row + 1 << ";" << world->player.col + 1 << "H";
}

void Saiph::inspect() {
	/* inspect the dungeon for interesting monsters/objects/places */
	for (int r = MAP_ROW_START; r <= MAP_ROW_END; ++r) {
		for (int c = 0; c < COLS; ++c) {
			unsigned char symbol = world->map[r][c];
			if (symbol == SOLID_ROCK) // unlit rooms makes floor go back to SOLID_ROCK
				symbol = map[current_branch][current_level].dungeon[r][c];
			for (vector<Analyzer *>::iterator a = analyzer_symbols[symbol].begin(); a != analyzer_symbols[symbol].end(); ++a)
				(*a)->analyze(r, c, symbol);
		}
	}
}

bool Saiph::isLegalMove(const Point &to, const Point &from) {
	/* we need to check this when creating and backtracking the pathmap:
	 * 1. if move is blacklisted, return false
	 * 2. if move is cardinal, return true (no special rules)
	 * 3. moving in/out of doors, return false
	 * 4. polymorphed to grid bug, return false
	 * 5. moving past two corners:
	 *    - in sokoban, return false
	 *    - at least one corner is a boulder, return true
	 *    - not carrying much, return true
	 *    - otherwise, return false
	 */
	// TODO: if (blacklisted_move) return false;
	if (to.row == from.row || to.col == from.col)
		return true; // all cardinal moves are legal
	if (map[current_branch][current_level].dungeon[to.row][to.col] == OPEN_DOOR || map[current_branch][current_level].dungeon[from.row][from.col] == OPEN_DOOR)
		return false; // in/out of door 
	// TODO: if (polymorphed_to_grid_bug) return false;
	if (!passable[map[current_branch][current_level].dungeon[to.row][from.col]] && !passable[map[current_branch][current_level].dungeon[from.row][to.col]]) {
		/* moving past two corners */
		// TODO: if (in_sokoban) return false;
		if (map[current_branch][current_level].dungeon[to.row][from.col] == BOULDER || map[current_branch][current_level].dungeon[from.row][to.col] == BOULDER)
			return true; // at least one corner is a boulder, we may pass
		// TODO? if (not_carrying_too_much) return true;
		return false;
	}
	return true;
}

void Saiph::updateMaps() {
	/* update the various maps */
	/* this loop is a small hack. FIXME: make own monster tracker that does this
	 * it removes unseen monsters next to the player which we think are blocking the path */
	for (int r = world->player.row - 1; r <= world->player.row + 1; ++r) {
		if (r < MAP_ROW_START || r > MAP_ROW_END)
			continue;
		for (int c = world->player.col - 1; c <= world->player.col + 1; ++c) {
			if (c < 0 || c > COLS)
				continue;
			if (map[current_branch][current_level].monster[r][c] != NOMONSTER)
				map[current_branch][current_level].monster[r][c] = NOMONSTER;
		}
	}
	for (int r = MAP_ROW_START; r <= MAP_ROW_END; ++r) {
		for (int c = 0; c < COLS; ++c) {
			unsigned char s = world->map[r][c];
			if (s == SOLID_ROCK)
				continue; // not interesting (also mess up unlit rooms)
			/* "static" dungean features */
			if (static_dungeon_symbol[s]) {
				/* update the map showing static stuff */
				map[current_branch][current_level].dungeon[r][c] = s;
			} else if (!static_dungeon_symbol[map[current_branch][current_level].dungeon[r][c]]) {
				/* hmm... this place isn't solid rock,
				 * nor can we see which "static dungeon symbol" should be here.
				 * most likely some item is in the way.
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
	/* create a path map used for finding shortest path */
	/* TODO: we can improve this further
	 * - instead of "backtracking" by "searching adjacent squares",
	 *   make a linked list, something like:
	 *   struct PathNode {
	 *       Point nextnode;
	 *       unsigned char move;
	 *   };
	 *   PathNode pathnodes[ROWS][COLS];
	 *
	 *   i'll get the idea when i read this later... */
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c)
			pathmap[r][c] = UINT_MAX;
	}
	Point from;
	from.row = world->player.row;
	from.col = world->player.col;
	pathing_queue[0] = from;
	pathmap[from.row][from.col] = 0;
	int curcost = 0;
	int curnode = 0;
	int nodes = 1;
	while (curnode < nodes) {
		from = pathing_queue[curnode];
		curcost = pathmap[from.row][from.col];
		Point to;
		for (to.row = from.row - 1; to.row <= from.row + 1; ++to.row) {
			if (to.row < MAP_ROW_START || to.row > MAP_ROW_END)
				continue;
			for (to.col = from.col - 1; to.col <= from.col + 1; ++to.col) {
				unsigned char ws = world->map[to.row][to.col];
				unsigned char s = map[current_branch][current_level].dungeon[to.row][to.col];
				if (to.col < 0 || to.col >= COLS)
					continue;
				else if (!passable[s])
					continue;
				else if (!passable[ws] && ws != SOLID_ROCK)
					continue;
				else if (monster[ws])
					continue; // can't path through monsters, for now
				else if (!isLegalMove(to, from))
					continue; // illegal diagonal move
				unsigned int newcost = curcost + ((to.row == from.row || to.col == from.col) ? COST_CARDINAL : COST_DIAGONAL);
				if (s == LAVA)
					newcost += COST_LAVA; // TODO: only if we levitate
				else if (s == WATER)
					newcost += COST_WATER; // TODO: only if we levitate/waterwalk
				else if (s == TRAP)
					newcost += COST_TRAP;
				else if (s == ICE)
					newcost += COST_ICE;
				if (ws == PET)
					newcost += COST_PET;
				if (newcost < pathmap[to.row][to.col]) {
					pathmap[to.row][to.col] = newcost;
					pathing_queue[nodes] = to;
					++nodes;
				}
			}
		}
		++curnode;
	}
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
