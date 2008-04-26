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

	/* pathing */
	pathcost = new unsigned int*[ROWS];
	for (int r = 0; r < ROWS; ++r) {
		pathcost[r] = new unsigned int[COLS];
		for (int c = 0; c < COLS; ++c)
			pathcost[r][c] = 0xffff;
	}
	pathpos = new char*[ROWS * COLS];
	for (int r = 0; r < ROWS * COLS; ++r) {
		pathpos[r] = new char[2];
		pathpos[r][0] = 0;
		pathpos[r][1] = 0;
	}
	passable = new char[MAX_PASSABLE];
	passable_count = 0;
	/* most frequent features should be first */
	passable[passable_count++] = FLOOR;
	passable[passable_count++] = CORRIDOR;
	passable[passable_count++] = WATER;
	passable[passable_count++] = OPEN_DOOR;
	passable[passable_count++] = STAIRS_UP;
	passable[passable_count++] = STAIRS_DOWN;
	passable[passable_count++] = FOUNTAIN;
	passable[passable_count++] = ALTAR;
	passable[passable_count++] = GRAVE;
	passable[passable_count++] = TRAP;
	passable[passable_count++] = SINK;
	passable[passable_count++] = THRONE;
	passable[passable_count++] = ICE;
	passable[passable_count++] = LAVA;
	passable[passable_count++] = LOWERED_DRAWBRIDGE;
	passable[passable_count++] = WEAPON;
	passable[passable_count++] = ARMOR;
	passable[passable_count++] = RING;
	passable[passable_count++] = AMULET;
	passable[passable_count++] = TOOL;
	passable[passable_count++] = FOOD;
	passable[passable_count++] = POTION;
	passable[passable_count++] = SCROLL;
	passable[passable_count++] = SPELLBOOK;
	passable[passable_count++] = WAND;
	passable[passable_count++] = GOLD;
	passable[passable_count++] = GEM;
	passable[passable_count++] = STATUE;
	passable[passable_count++] = IRON_BALL;
	passable[passable_count++] = CHAINS;
	passable[passable_count++] = VENOM;

	/* branches */
	branches = new Branch*[MAX_BRANCHES];
	current_branch = BRANCH_MAIN;
	for (int b = 0; b < MAX_BRANCHES; ++b) {
		branches[b] = new Branch;
		memset(branches[b]->map, ' ', MAX_DUNGEON_DEPTH * ROWS * COLS);
		memset(branches[b]->search, '\0', MAX_DUNGEON_DEPTH * ROWS * COLS);
		memset(branches[b]->unpassable, 1, MAX_DUNGEON_DEPTH * ROWS * COLS);
		memset(branches[b]->diagonally_unpassable, 1, MAX_DUNGEON_DEPTH * ROWS * COLS);
	}

	/* messages */
	messages = new string("");

	/* Analyzers */
	analyzers = new Analyzer*[MAX_ANALYZERS];
	analyzer_count = 0;
	analyzers[analyzer_count++] = (Analyzer *) new DoorAnalyzer(this);
	analyzers[analyzer_count++] = (Analyzer *) new FoodAnalyzer(this);
	analyzers[analyzer_count++] = (Analyzer *) new ExploreAnalyzer(this);
	analyzers[analyzer_count++] = (Analyzer *) new HealthAnalyzer(this);
	analyzers[analyzer_count++] = (Analyzer *) new LevelAnalyzer(this);
	analyzers[analyzer_count++] = (Analyzer *) new LootAnalyzer(this);
	analyzers[analyzer_count++] = (Analyzer *) new MonsterAnalyzer(this);
}

/* destructors */
Saiph::~Saiph() {
	for (int a = 0; a < analyzer_count; ++a)
		delete analyzers[a];
	delete [] analyzers;
	for (int b = 0; b < MAX_BRANCHES; ++b)
		delete branches[b];
	delete [] branches;
	for (int p = 0; p < ROWS; ++p)
		delete [] pathcost[p];
	delete [] pathcost;
	for (int p = 0; p < (ROWS * COLS); ++p)
		delete [] pathpos[p];
	delete [] pathpos;
	delete [] passable;
	delete history;
	delete messages;
	delete world;
	delete connection;
}

/* methods */
void Saiph::directionToPos(char direction, int &to_row, int &to_col) {
	/* return (by setting to_row & to_col) the position by the given direction */
	switch (direction) {
		case MOVE_NW:
			--to_row;
			--to_col;
			break;

		case MOVE_N:
			--to_row;
			break;

		case MOVE_NE:
			--to_row;
			++to_col;
			break;

		case MOVE_W:
			--to_col;
			break;

		case MOVE_E:
			++to_col;
			break;

		case MOVE_SW:
			++to_row;
			--to_col;
			break;

		case MOVE_S:
			++to_row;
			break;

		case MOVE_SE:
			++to_row;
			++to_col;
			break;

		default:
			cerr << "invalid direction: " << direction << endl;
			break;
	}
}

void Saiph::dumpMaps() {
	/* search map */
	cout << (char) 27 << "[26;1H";
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c) {
			cout << (char) (branches[current_branch]->search[world->player.dungeon][r][c] % 96 + 32);
			//cout << (int) branches[current_branch]->unpassable[world->player.dungeon][r][c];
			//cout << (int) branches[current_branch]->diagonally_unpassable[world->player.dungeon][r][c];
		}
		cout << endl;
	}
	/* world map as the bot sees it */
	for (int r = 0; r < ROWS; ++r) {
		cout << (char) 27 << "[" << r + 1 << ";82H";
		for (int c = 0; c < COLS; ++c) {
			cout << (char) (branches[current_branch]->map[world->player.dungeon][r][c]);
		}
	}
	/* path map */
	for (int r = 0; r < ROWS; ++r) {
		cout << (char) 27 << "[" << r + 26 << ";82H";
		for (int c = 0; c < COLS; ++c) {
			cout << (char) (pathcost[r][c] % 96 + 32);
		}
	}
	/* return cursor back to player */
	cout << (char) 27 << "[" << world->player.row + 1 << ";" << world->player.col + 1 << "H";
}

void Saiph::farlook(int row, int col) {
	/* look at something, eg. monster */
	char command[ROWS + COLS + 3];
	int pos = 0;
	command[pos++] = ';';
	int cursor_row = world->player.row;
	int cursor_col = world->player.col;
	while (cursor_row != row && cursor_col != col) {
		char move = moveToDirection(row, col, cursor_row, cursor_col);
		directionToPos(move, cursor_row, cursor_col);
		command[pos++] = move;
	}
	command[pos++] = ',';
	command[pos] = '\0';
	cerr << command << endl;
	world->command(command);
}

bool Saiph::isLegalMove(int branch, int dungeon, int to_row, int to_col, int from_row, int from_col) {
	/* check if the move is legal */
	int rd = abs(to_row - from_row);
	int cd = abs(to_col - from_col);
	if (rd > 1 || cd > 1)
		return false; // moving more than 1 square
	else if (branches[branch]->unpassable[dungeon][to_row][to_col] != 0)
		return false; // can't move to this square
	else if (rd == 1 && cd == 1) {
		/* diagonal move */
		if (branches[branch]->map[dungeon][from_row][from_col] == OPEN_DOOR || branches[branch]->map[dungeon][to_row][to_col] == OPEN_DOOR)
			return false; // can't move diagonally into or out of door
		else if (branches[branch]->diagonally_unpassable[dungeon][to_row][from_col] != 0 && branches[branch]->diagonally_unpassable[dungeon][from_row][to_col] != 0)
			return false; // can't move diagonally when both sides are blocked
	}
	return true;
}

bool Saiph::monsterOnSquare(int row, int col) {
	char s = world->map[row][col];
	return ((s >= '@' && s <= 'Z') || (s >= 'a' && s <= 'z') || s == '&' || s == '\'' || s == '6' || s == ':' || s == ';' || s == '~');
}

char Saiph::moveToDirection(int to_row, int to_col, int from_row, int from_col) {
	/* return the direction by the given move */
	if (from_row < to_row && from_col < to_col)
		return MOVE_SE;
	if (from_row < to_row && from_col > to_col)
		return MOVE_SW;
	if (from_row > to_row && from_col < to_col)
		return MOVE_NE;
	if (from_row > to_row && from_col > to_col)
		return MOVE_NW;
	if (from_row < to_row)
		return MOVE_S;
	if (from_row > to_row)
		return MOVE_N;
	if (from_col < to_col)
		return MOVE_E;
	if (from_col > to_col)
		return MOVE_W;
	cerr << "invalid move: " << from_row << ", " << from_col << " -> " << to_row << ", " << to_col << endl;
	return -1;
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
	*messages = world->messages;
	cerr << *messages << endl;
	for (int a = 0; a < analyzer_count; ++a) {
		int priority = analyzers[a]->parseMessages(messages);
		if (priority > command.priority) {
			command.analyzer = a;
			command.priority = priority;
		}
	}

	/* call start() in analyzers */
	if (!world->question && !world->menu) {
		for (int a = 0; a < analyzer_count; ++a) {
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
		for (int a = 0; a < analyzer_count; ++a) {
			int priority = analyzers[a]->finish();
			if (priority > command.priority) {
				command.analyzer = a;
				command.priority = priority;
			}
		}
	}

	if (world->question && command.analyzer == -1) {
		cerr << "Unhandled question: " << *messages << endl;
		return false;
	}

	if (world->menu && command.analyzer == -1) {
		cerr << "Unhandled menu: " << *messages << endl;
		return false;
	}

	cerr << "letting analyzer " << command.analyzer << " do its thing with priority " << command.priority << endl;

	/* check if we got a command */
	if (command.analyzer == -1)
		return false;

	/* let an analyzer do its command */
	analyzers[command.analyzer]->command();
	return true;
}

char Saiph::shortestPath(int row, int col, bool allow_illegal_last_move, int &distance, bool &direct_line) {
	/* attempt to find the shortest path to destination.
	 * returns move or -1 if unable to find a path */
	/* allow_illegal_last_move:
	 * if this is true we'll "move" diagonally through doors if it's the last move.
	 * why? because monsters may stand in a door way, making it possible to attack them */
	distance = 0;
	direct_line = true;
	if (row <= MAP_ROW_START || row >= MAP_ROW_END || col < 1 || col > COLS - 1)
		return -1; // this means we can't move to the edges of the map (hopefully "ok")
	unsigned int curcost = pathcost[row][col];
	if (curcost == UINT_MAX && !allow_illegal_last_move)
		return -1; // can't move here
	if (curcost == 0)
		return REST; // we're standing on the pos we're looking for?
	unsigned int antiloop = curcost - 666; // any other value but curcost :)
	char move = -1;
	char prevmove = -1;
	while (curcost > 0 && antiloop != curcost) {
		int r = row;
		int c = col;
		antiloop = curcost; // if curcost doesn't change the loop will end
		if (pathcost[row - 1][col - 1] < curcost && ((allow_illegal_last_move && prevmove == -1) || isLegalMove(current_branch, world->player.dungeon, row - 1, col - 1, row, col))) {
			move = MOVE_SE;
			r = row - 1;
			c = col - 1;
			curcost = pathcost[r][c];
		}
		if (pathcost[row - 1][col] < curcost) {
			move = MOVE_S;
			r = row - 1;
			c = col;
			curcost = pathcost[r][c];
		}
		if (pathcost[row - 1][col + 1] < curcost && ((allow_illegal_last_move && prevmove == -1) || isLegalMove(current_branch, world->player.dungeon, row - 1, col + 1, row, col))) {
			move = MOVE_SW;
			r = row - 1;
			c = col + 1;
			curcost = pathcost[r][c];
		}
		if (pathcost[row][col - 1] < curcost) {
			move = MOVE_E;
			r = row;
			c = col - 1;
			curcost = pathcost[r][c];
		}
		if (pathcost[row][col + 1] < curcost) {
			move = MOVE_W;
			r = row;
			c = col + 1;
			curcost = pathcost[r][c];
		}
		if (pathcost[row + 1][col - 1] < curcost && ((allow_illegal_last_move && prevmove == -1) || isLegalMove(current_branch, world->player.dungeon, row + 1, col - 1, row, col))) {
			move = MOVE_NE;
			r = row + 1;
			c = col - 1;
			curcost = pathcost[r][c];
		}
		if (pathcost[row + 1][col] < curcost) {
			move = MOVE_N;
			r = row + 1;
			c = col;
			curcost = pathcost[r][c];
		}
		if (pathcost[row + 1][col + 1] < curcost && ((allow_illegal_last_move && prevmove == -1) || isLegalMove(current_branch, world->player.dungeon, row + 1, col + 1, row, col))) {
			move = MOVE_NW;
			r = row + 1;
			c = col + 1;
			curcost = pathcost[r][c];
		}
		if (distance > 0 && prevmove != move)
			direct_line = false;
		prevmove = move;
		++distance;
		row = r;
		col = c;
	}
	if (curcost > 0)
		return -1;
	return move;
}

/* private methods */
void Saiph::inspect() {
	/* inspect the dungeon for interesting monsters/objects/places */
	for (int r = MAP_ROW_START; r <= MAP_ROW_END; ++r) {
		for (int c = 0; c < COLS; ++c) {
			char symbol = world->map[r][c];
			if (symbol == SOLID_ROCK) // unlit rooms makes floor go back to SOLID_ROCK
				symbol = branches[current_branch]->map[world->player.dungeon][r][c];
			for (int a = 0; a < analyzer_count; ++a) {
				for (int s = 0; s < analyzers[a]->symbol_count; ++s) {
					if (analyzers[a]->symbols[s] == symbol)
						analyzers[a]->analyze(r, c, symbol);
				}
			}
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
			if (branches[current_branch]->unpassable[world->player.dungeon][r][c] == 2)
				branches[current_branch]->unpassable[world->player.dungeon][r][c] = 0;
		}
	}
	for (int r = MAP_ROW_START; r <= MAP_ROW_END; ++r) {
		for (int c = 0; c < COLS; ++c) {
			char s = world->map[r][c];
			if (s == SOLID_ROCK)
				continue;
			/* using pointer next as we need updated map for [diagonally_]unpassable */
			char *hs = &branches[current_branch]->map[world->player.dungeon][r][c];
			if (s == VERTICAL_WALL || s == HORIZONTAL_WALL || s == FLOOR || s == OPEN_DOOR || s == CLOSED_DOOR || s == IRON_BARS || s == TREE || s == CORRIDOR || s == STAIRS_UP || s == STAIRS_DOWN || s == ALTAR || s == GRAVE || s == THRONE || s == SINK || s == FOUNTAIN || s == WATER || s == ICE || s == LAVA || s == LOWERED_DRAWBRIDGE || s == RAISED_DRAWBRIDGE || s == TRAP) {
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
			if (branches[current_branch]->unpassable[world->player.dungeon][r][c] != 2) // 2 means it's blocked by eg. monster
				branches[current_branch]->unpassable[world->player.dungeon][r][c] = (*hs == VERTICAL_WALL || *hs == HORIZONTAL_WALL || *hs == CLOSED_DOOR || *hs == IRON_BARS || *hs == TREE || *hs == RAISED_DRAWBRIDGE || s == BOULDER) ? 1 : 0;
			/* diagonally unpassable map */
			branches[current_branch]->diagonally_unpassable[world->player.dungeon][r][c] = (*hs == VERTICAL_WALL || *hs == HORIZONTAL_WALL || *hs == CLOSED_DOOR || *hs == IRON_BARS || *hs == TREE || *hs == RAISED_DRAWBRIDGE) ? 1 : 0;
		}
	}

	updatePathMap();
}

void Saiph::updatePathMap() {
	/* create a path map used for finding shortest path */
	/* is this dijkstra? */
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c)
			pathcost[r][c] = UINT_MAX;
	}
	int row = world->player.row;
	int col = world->player.col;
	pathcost[row][col] = 0;
	int nextnode = 0;
	int nodes = 1;
	pathpos[nextnode][0] = row;
	pathpos[nextnode][1] = col;
	int curcost = 0;
	while (nextnode < nodes) {
		row = pathpos[nextnode][0];
		col = pathpos[nextnode][1];
		curcost = pathcost[row][col];
		for (int r = row - 1; r <= row + 1; ++r) {
			if (r < MAP_ROW_START || r > MAP_ROW_END)
				continue;
			for (int c = col - 1; c <= col + 1; ++c) {
				if (c < 0 || c >= COLS)
					continue;
				if (!isLegalMove(current_branch, world->player.dungeon, r, c, row, col))
					continue;
				if (monsterOnSquare(r, c))
					continue; // can't path through monsters, for now
				char s = branches[current_branch]->map[world->player.dungeon][r][c];
				unsigned int newpathcost = curcost + ((r == row || c == col) ? COST_CARDINAL : COST_DIAGONAL);
				if (s == LAVA)
					newpathcost += COST_LAVA;
				else if (s == TRAP)
					newpathcost += COST_TRAP;
				else if (s == WATER)
					newpathcost += COST_WATER;
				if (world->map[r][c] == PET)
					newpathcost += COST_PET;
				if (newpathcost < pathcost[r][c]) {
					pathcost[r][c] = newpathcost;
					pathpos[nodes][0] = r;
					pathpos[nodes][1] = c;
					++nodes;
				}
			}
		}
		++nextnode;
	}
}

/* main */
int main() {
	Saiph saiph(false);
	//for (int a = 0; a < 200 && saiph.run(); ++a)
	//	;
	while (saiph.run())
		;
}
