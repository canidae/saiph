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

	/* history */
	history = new list<Dungeon>;

	/* branches */
	branches = new Branch*[MAX_BRANCHES];
	current_branch = BRANCH_MAIN;
	for (int b = 0; b < MAX_BRANCHES; ++b) {
		branches[b] = new Branch;
		memset(branches[b]->map, ' ', MAX_DUNGEON_DEPTH * ROWS * COLS);
		memset(branches[b]->search, '\0', MAX_DUNGEON_DEPTH * ROWS * COLS);
		memset(branches[b]->unpassable, BLOCKED, MAX_DUNGEON_DEPTH * ROWS * COLS);
		memset(branches[b]->diagonally_unpassable, BLOCKED, MAX_DUNGEON_DEPTH * ROWS * COLS);
	}

	/* monsters */
	for (int m = 0; m <= UCHAR_MAX; ++m) {
		if ((m >= '@' && m <= 'Z') || (m >= 'a' && m <= 'z') || m == '&' || m == '\'' || m == '6' || m == ':' || m == ';' || m == '~')
			ismonster[m] = true;
		else
			ismonster[m] = false;
	}

	/* pathing & maps */
	for (int s = 0; s <= UCHAR_MAX; ++s) {
		passable[s] = true;
		diagonally_passable[s] = true;
		static_dungeon_symbol[s] = false;
	}
	passable[(unsigned char) VERTICAL_WALL] = false;
	passable[(unsigned char) HORIZONTAL_WALL] = false;
	passable[(unsigned char) CLOSED_DOOR] = false;
	passable[(unsigned char) IRON_BARS] = false;
	passable[(unsigned char) TREE] = false;
	passable[(unsigned char) RAISED_DRAWBRIDGE] = false;
	passable[(unsigned char) BOULDER] = false;
	diagonally_passable[(unsigned char) VERTICAL_WALL] = false;
	diagonally_passable[(unsigned char) HORIZONTAL_WALL] = false;
	diagonally_passable[(unsigned char) CLOSED_DOOR] = false;
	diagonally_passable[(unsigned char) IRON_BARS] = false;
	diagonally_passable[(unsigned char) TREE] = false;
	diagonally_passable[(unsigned char) RAISED_DRAWBRIDGE] = false;
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
	for (int b = 0; b < MAX_BRANCHES; ++b)
		delete branches[b];
	delete [] branches;
	delete history;
	delete world;
	delete connection;
}

/* methods */
Point Saiph::directionToPos(unsigned char direction, Point target) {
	/* return the new position after moving given direction */
	switch (direction) {
		case MOVE_NW:
			--target.row;
			--target.col;
			break;

		case MOVE_N:
			--target.row;
			break;

		case MOVE_NE:
			--target.row;
			++target.col;
			break;

		case MOVE_W:
			--target.col;
			break;

		case MOVE_E:
			++target.col;
			break;

		case MOVE_SW:
			++target.row;
			--target.col;
			break;

		case MOVE_S:
			++target.row;
			break;

		case MOVE_SE:
			++target.row;
			++target.col;
			break;

		default:
			cerr << "invalid direction: " << direction << endl;
			break;
	}
	return target;
}

void Saiph::dumpMaps() {
	/* search map */
	cout << (unsigned char) 27 << "[26;1H";
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c) {
			cout << (unsigned char) (branches[current_branch]->search[world->player.dungeon][r][c] % 96 + 32);
			//cout << (int) branches[current_branch]->unpassable[world->player.dungeon][r][c];
			//cout << (int) branches[current_branch]->diagonally_unpassable[world->player.dungeon][r][c];
		}
		cout << endl;
	}
	/* world map as the bot sees it */
	for (int r = 0; r < ROWS; ++r) {
		cout << (unsigned char) 27 << "[" << r + 1 << ";82H";
		for (int c = 0; c < COLS; ++c) {
			cout << (unsigned char) (branches[current_branch]->map[world->player.dungeon][r][c]);
		}
	}
	/* path map */
	for (int r = 0; r < ROWS; ++r) {
		cout << (unsigned char) 27 << "[" << r + 26 << ";82H";
		for (int c = 0; c < COLS; ++c) {
			cout << (unsigned char) (pathcost[r][c] % 96 + 32);
		}
	}
	/* return cursor back to player */
	cout << (unsigned char) 27 << "[" << world->player.row + 1 << ";" << world->player.col + 1 << "H";
}

void Saiph::farlook(const Point &target) {
	/* look at something, eg. monster */
	world->command.push_back(';');
	Point cursor;
	cursor.row = world->player.row;
	cursor.col = world->player.col;
	while (cursor.row != target.row && cursor.col != target.col) {
		unsigned char move;
		if (cursor.row < target.row && cursor.col < target.col)
			move = MOVE_SE;
		else if (cursor.row < target.row && cursor.col > target.col)
			move = MOVE_SW;
		else if (cursor.row > target.row && cursor.col < target.col)
			move = MOVE_NE;
		else if (cursor.row > target.row && cursor.col > target.col)
			move = MOVE_NW;
		else if (cursor.row < target.row)
			move = MOVE_S;
		else if (cursor.row > target.row)
			move = MOVE_N;
		else if (cursor.col < target.col)
			move = MOVE_E;
		else
			move = MOVE_W;
		cursor = directionToPos(move, cursor);
		world->command.push_back(move);
	}
	world->command.push_back(',');
	cerr << world->command << endl;
	world->executeCommand();
}

bool Saiph::isLegalMove(const Point &to, const Point &from) {
	/* check if the move is legal */
	int rd = abs(to.row - from.row);
	int cd = abs(to.col - from.col);
	if (rd > 1 || cd > 1)
		return false; // moving more than 1 square
	else if (branches[current_branch]->unpassable[world->player.dungeon][to.row][to.col] != NOT_BLOCKED)
		return false; // can't move to this square
	else if (rd == 1 && cd == 1) {
		/* diagonal move */
		if (branches[current_branch]->map[world->player.dungeon][from.row][from.col] == OPEN_DOOR || branches[current_branch]->map[world->player.dungeon][to.row][to.col] == OPEN_DOOR)
			return false; // can't move diagonally into or out of door
		else if (branches[current_branch]->diagonally_unpassable[world->player.dungeon][to.row][from.col] != NOT_BLOCKED && branches[current_branch]->diagonally_unpassable[world->player.dungeon][from.row][to.col] != NOT_BLOCKED)
			return false; // can't move diagonally when both sides are blocked
	}
	return true;
}

void Saiph::registerAnalyzerSymbols(Analyzer *analyzer, const vector<unsigned char> &symbols) {
	/* register which symbols analyzers should analyze */
	for (vector<unsigned char>::const_iterator s = symbols.begin(); s != symbols.end(); ++s)
		analyzer_symbols[*s].push_back(analyzer);
}

bool Saiph::run() {
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

	/* save dungeon in history */
	history->push_front(*world);
	while (history->size() > MAX_HISTORY)
		history->pop_back();

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
	unsigned int curcost = pathcost[target.row][target.col];
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
		if (pathcost[to.row][to.col] < curcost && (allow_illegal_last_move || isLegalMove(to, from))) {
			move = MOVE_SE;
			curcost = pathcost[to.row][to.col];
			next = to;
		}
		++to.col;
		if (pathcost[to.row][to.col] < curcost && (allow_illegal_last_move || isLegalMove(to, from))) {
			move = MOVE_S;
			curcost = pathcost[to.row][to.col];
			next = to;
		}
		++to.col;
		if (pathcost[to.row][to.col] < curcost && (allow_illegal_last_move || isLegalMove(to, from))) {
			move = MOVE_SW;
			curcost = pathcost[to.row][to.col];
			next = to;
		}
		++to.row;
		if (pathcost[to.row][to.col] < curcost && (allow_illegal_last_move || isLegalMove(to, from))) {
			move = MOVE_W;
			curcost = pathcost[to.row][to.col];
			next = to;
		}
		++to.row;
		if (pathcost[to.row][to.col] < curcost && (allow_illegal_last_move || isLegalMove(to, from))) {
			move = MOVE_NW;
			curcost = pathcost[to.row][to.col];
			next = to;
		}
		--to.col;
		if (pathcost[to.row][to.col] < curcost && (allow_illegal_last_move || isLegalMove(to, from))) {
			move = MOVE_N;
			curcost = pathcost[to.row][to.col];
			next = to;
		}
		--to.col;
		if (pathcost[to.row][to.col] < curcost && (allow_illegal_last_move || isLegalMove(to, from))) {
			move = MOVE_NE;
			curcost = pathcost[to.row][to.col];
			next = to;
		}
		--to.row;
		if (pathcost[to.row][to.col] < curcost && (allow_illegal_last_move || isLegalMove(to, from))) {
			move = MOVE_E;
			curcost = pathcost[to.row][to.col];
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
void Saiph::inspect() {
	/* inspect the dungeon for interesting monsters/objects/places */
	for (int r = MAP_ROW_START; r <= MAP_ROW_END; ++r) {
		for (int c = 0; c < COLS; ++c) {
			unsigned char symbol = world->map[r][c];
			if (symbol == SOLID_ROCK) // unlit rooms makes floor go back to SOLID_ROCK
				symbol = branches[current_branch]->map[world->player.dungeon][r][c];
			for (vector<Analyzer *>::iterator a = analyzer_symbols[symbol].begin(); a != analyzer_symbols[symbol].end(); ++a)
				(*a)->analyze(r, c, symbol);
		}
	}
}

void Saiph::updateMaps() {
	/* update the various maps */
	/* this loop is a small hack.
	 * it removes unseen monsters next to the player which we think are blocking the path */
	for (int r = world->player.row - 1; r <= world->player.row + 1; ++r) {
		if (r < MAP_ROW_START || r > MAP_ROW_END)
			continue;
		for (int c = world->player.col - 1; c <= world->player.col + 1; ++c) {
			if (c < 0 || c > COLS)
				continue;
			if (branches[current_branch]->unpassable[world->player.dungeon][r][c] == TEMPORARY_BLOCKED)
				branches[current_branch]->unpassable[world->player.dungeon][r][c] = NOT_BLOCKED;
		}
	}
	for (int r = MAP_ROW_START; r <= MAP_ROW_END; ++r) {
		for (int c = 0; c < COLS; ++c) {
			unsigned char s = world->map[r][c];
			if (s == SOLID_ROCK)
				continue;
			/* using pointer as we need updated map for [diagonally_]unpassable */
			unsigned char *hs = &branches[current_branch]->map[world->player.dungeon][r][c];
			if (static_dungeon_symbol[s]) {
				/* "static" dungeon features (doors may be destroyed, though).
				 * update the map showing static stuff */
				branches[current_branch]->map[world->player.dungeon][r][c] = s;
			} else if (*hs == SOLID_ROCK) {
				/* "dynamic" stuff that can disappear on a spot we've never seen before.
				 * pretend there's an open door here until we see otherwise */
				branches[current_branch]->map[world->player.dungeon][r][c] = OPEN_DOOR;
			} else if (*hs == CLOSED_DOOR) {
				/* there used to be a door here, but now something else is here.
				 * it's quite possible a monster opened the door */
				branches[current_branch]->map[world->player.dungeon][r][c] = OPEN_DOOR;
			} else if (*hs == HORIZONTAL_WALL || *hs == VERTICAL_WALL) {
				/* there used to be a wall here, but isn't any longer.
				 * make it an open door for the time being */
				branches[current_branch]->map[world->player.dungeon][r][c] = OPEN_DOOR;
			}
			/* unpassable map */
			if (branches[current_branch]->unpassable[world->player.dungeon][r][c] != TEMPORARY_BLOCKED)
				branches[current_branch]->unpassable[world->player.dungeon][r][c] = (passable[*hs] ? NOT_BLOCKED : BLOCKED);
			/* diagonally unpassable map */
			branches[current_branch]->diagonally_unpassable[world->player.dungeon][r][c] = (diagonally_passable[*hs] ? NOT_BLOCKED : BLOCKED);
		}
	}

	updatePathMap();
}

void Saiph::updatePathMap() {
	/* create a path map used for finding shortest path */
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c)
			pathcost[r][c] = UINT_MAX;
	}
	Point from;
	from.row = world->player.row;
	from.col = world->player.col;
	pathing_queue[0] = from;
	pathcost[from.row][from.col] = 0;
	int curcost = 0;
	int curnode = 0;
	int nodes = 1;
	while (curnode < nodes) {
		from = pathing_queue[curnode];
		curcost = pathcost[from.row][from.col];
		Point to;
		for (to.row = from.row - 1; to.row <= from.row + 1; ++to.row) {
			if (to.row < MAP_ROW_START || to.row > MAP_ROW_END)
				continue;
			for (to.col = from.col - 1; to.col <= from.col + 1; ++to.col) {
				unsigned char ws = world->map[to.row][to.col];
				if (to.col < 0 || to.col >= COLS)
					continue;
				else if (!isLegalMove(to, from))
					continue;
				else if (ismonster[ws])
					continue; // can't path through monsters, for now
				unsigned char s = branches[current_branch]->map[world->player.dungeon][to.row][to.col];
				unsigned int newpathcost = curcost + ((to.row == from.row || to.col == from.col) ? COST_CARDINAL : COST_DIAGONAL);
				if (s == LAVA)
					newpathcost += COST_LAVA; // TODO: only if we levitate
				else if (s == WATER)
					newpathcost += COST_WATER; // TODO: only if we levitate/waterwalk
				else if (s == TRAP)
					newpathcost += COST_TRAP;
				if (ws == PET)
					newpathcost += COST_PET;
				if (newpathcost < pathcost[to.row][to.col]) {
					pathcost[to.row][to.col] = newpathcost;
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
