#include "Saiph.h"

/* constructors */
Saiph::Saiph(bool remote) {
	connection = new Connection(remote);
	world = new World(connection);

	/* next command */
	memset(command.command, '\0', MAX_COMMAND_LENGTH);
	command.priority = 0;

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
	}

	/* message parser */
	parser = new MessageParser(this);

	/* Analyzers */
	analyzers = new Analyzer*[MAX_ANALYZERS];
	analyzer_count = 0;
	analyzers[analyzer_count++] = dynamic_cast<Analyzer*>(new DoorAnalyzer(this));
	analyzers[analyzer_count++] = dynamic_cast<Analyzer*>(new DungeonAnalyzer(this));
	analyzers[analyzer_count++] = dynamic_cast<Analyzer*>(new ExploreAnalyzer(this));
	analyzers[analyzer_count++] = dynamic_cast<Analyzer*>(new FightAnalyzer(this));
	analyzers[analyzer_count++] = dynamic_cast<Analyzer*>(new HealthAnalyzer(this));
	analyzers[analyzer_count++] = dynamic_cast<Analyzer*>(new LevelAnalyzer(this));
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
	delete parser;
	delete world;
	delete connection;
}

/* methods */
void Saiph::dumpScreens() {
	/* world map */
	cout << world->data;
	/* search map */
	cout << (char) 27 << "[26;1H";
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c) {
			cout << (char) (branches[current_branch]->search[world->player.status.dungeon][r][c] % 96 + 32);
		}
		cout << endl;
	}
	/* world map as the bot sees it */
	for (int r = 0; r < ROWS; ++r) {
		cout << (char) 27 << "[" << r + 1 << ";82H";
		for (int c = 0; c < COLS; ++c) {
			cout << (char) (branches[current_branch]->map[world->player.status.dungeon][r][c]);
		}
	}
	/* last path */
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
	while (cursor_row != row && cursor_col != col)
		command[pos++] = findNextDirection(row, col, cursor_row, cursor_col);
	command[pos++] = ',';
	command[pos] = '\0';
	cerr << command << endl;
	world->command(command);
}

char Saiph::findNextDirection(const int to_row, const int to_col, int &from_row, int &from_col) {
	/* finds the next key we must press to move the cursor.
	 * this ignores walls & such!
	 * this will also change from_row/from_col! */
	if (from_row < to_row && from_col < to_col) {
		from_row++;
		from_col++;
		return MOVE_SE;
	}
	if (from_row < to_row && from_col > to_col) {
		from_row++;
		from_col--;
		return MOVE_SW;
	}
	if (from_row > to_row && from_col < to_col) {
		from_row--;
		from_col++;
		return MOVE_NE;
	}
	if (from_row > to_row && from_col > to_col) {
		from_row--;
		from_col--;
		return MOVE_NW;
	}
	if (from_row < to_row) {
		from_row++;
		return MOVE_S;
	}
	if (from_row > to_row) {
		from_row--;
		return MOVE_N;
	}
	if (from_col < to_col) {
		from_col++;
		return MOVE_E;
	}
	if (from_col > to_col) {
		from_col--;
		return MOVE_W;
	}
	return -1;
}

bool Saiph::run() {
	/* create the path map */
	updatePathMap();

	/* print stuff so we see what we're doing */
	dumpScreens();

	/* reset command */
	memset(command.command, '\0', MAX_COMMAND_LENGTH);
	command.priority = 0;

	/* save dungeon in history */

	/* deal with messages */
	parser->parse();

	/* call start() in analyzers */
	for (int a = 0; a < analyzer_count; ++a)
		analyzers[a]->start();

	/* inspect the dungeon */
	inspect();

	/* call finish() in analyzers */
	for (int a = 0; a < analyzer_count; ++a)
		analyzers[a]->finish();

	/* call end() in analyzers */
	for (int a = 0; a < analyzer_count; ++a)
		analyzers[a]->end();

	/* if no command, quit for now */
	if (command.priority == 0) {
		cout << (unsigned char) 27 << "[51;1H";
		return false;
	}

	/* do the selected move */
	cerr << "Command: " << command.command << " (priority: " << command.priority << ")" << endl;
	world->command(command.command);
	return true;
}

void Saiph::setNextCommand(const char *command, int priority) {
	/* priority range from 0 to 100.
	 * each analyzer set their own priorities.
	 * this is done so each analyzer may overrule another */
	/* FIXME
	 * no range check on the char array? tsk, tsk.
	 * might be an idea to rethink this stuff anyways */
	if (priority < this->command.priority)
		return;
	strcpy(this->command.command, command);
	this->command.priority = priority;
}

char Saiph::shortestPath(int row, int col, bool allow_illegal_last_move, int &distance, bool &direct_line) {
	/* attempt to find the shortest path to destination.
	 * returns move or -1 if unable to find a path */
	/* allow_illegal_last_move:
	 * if this is true we'll "move" diagonally through doors if it's the last move.
	 * why? because monsters may stand in a door way, making it possible to attack them */
	distance = 0;
	direct_line = true;
	if (row < MAP_ROW_START || row >= MAP_ROW_END || col < 1 || col > COLS - 1)
		return -1;
	unsigned int curcost = pathcost[row][col];
	unsigned int antiloop = curcost - 666; // any other value but curcost :)
	char move = -1;
	char prevmove = -1;
	while (curcost > 0 && antiloop != curcost) {
		int r = row;
		int c = col;
		char s = branches[current_branch]->map[world->player.status.dungeon][row][col];
		antiloop = curcost; // if curcost doesn't change the loop will end
		if (pathcost[row - 1][col - 1] < curcost && ((allow_illegal_last_move && prevmove == -1) || (s != OPEN_DOOR && branches[current_branch]->map[world->player.status.dungeon][row - 1][col - 1] != OPEN_DOOR))) {
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
		if (pathcost[row - 1][col + 1] < curcost && ((allow_illegal_last_move && prevmove == -1) || (s != OPEN_DOOR && branches[current_branch]->map[world->player.status.dungeon][row - 1][col + 1] != OPEN_DOOR))) {
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
		if (pathcost[row + 1][col - 1] < curcost && ((allow_illegal_last_move && prevmove == -1) || (s != OPEN_DOOR && branches[current_branch]->map[world->player.status.dungeon][row + 1][col - 1] != OPEN_DOOR))) {
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
		if (pathcost[row + 1][col + 1] < curcost && ((allow_illegal_last_move && prevmove == -1) || (s != OPEN_DOOR && branches[current_branch]->map[world->player.status.dungeon][row + 1][col + 1] != OPEN_DOOR))) {
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
	/* inspect the dungeon for interesting monsters/objects/places
	 * iterations:
	 * r: 20 ->      20
	 * c: 80 ->    1600
	 * a: 32 ->   51200
	 * s: 64 -> 3276800
	 * so try not to track unneeded symbols.
	 */
	char symbol;
	for (int r = MAP_ROW_START; r <= MAP_ROW_END; ++r) {
		for (int c = 0; c < COLS; ++c) {
			symbol = world->map[r][c];
			for (int a = 0; a < analyzer_count; ++a) {
				for (int s = 0; s < analyzers[a]->symbol_count; ++s) {
					if (analyzers[a]->symbols[s] == symbol)
						analyzers[a]->analyze(r, c, symbol);
				}
			}
		}
	}
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
		char ds = branches[current_branch]->map[world->player.status.dungeon][row][col];
		for (int r = row - 1; r <= row + 1; ++r) {
			if (r < MAP_ROW_START || r >= MAP_ROW_END)
				continue;
			for (int c = col - 1; c <= col + 1; ++c) {
				if (c < 0 || c >= COLS)
					continue;
				char s = world->map[r][c];
				bool hindrance = true;
				for (int u = 0; u < passable_count; ++u) {
					if (s == passable[u]) {
						hindrance = false;
						break;
					}
				}
				if (hindrance)
					continue;
				/* doors are special, no diagonal entrance/exit.
				 * even if it seems this isn't needed, it is!
				 * when there's a trap in front of a door the trap cost will make
				 * it impossible to move any further. as an example, let's say we're
				 * in the corridor leading to this room and we want that wand:
				 *  -----|
				 *  |.../|
				 * #-^...|
				 *  |....|
				 *  -----|
				 * in path map without this if:
				 *  ------
				 *  |579b|
				 * 0298ad|
				 *  |579b|
				 *  ------
				 * the pathing works by looking at the cost on the square of the wand,
				 * in this case the cost is "b" (11 in decimal (duh)). from that point it
				 * will go towards the lowest cost, in this case "9" straight to the left.
				 * further from "9" it will go to "7", to the left again, being the lowest cost.
				 * then it will go to "5". however, on square with "2" there's a door,
				 * and we can't move diagonally into that door, and every other direction cost
				 * more. thus, it'll stop here.
				 * if we make the cost map look like this instead:
				 *  ------
				 *  |bceg|
				 * 029bdf|
				 *  |bceg|
				 *  ------
				 * then it will move like this: g, d, b, 9, 2, 0
				 * we'll still need to check for doors when backtracing, though:
				 * let's say we're standing on either of the "b" instead, the cost for moving
				 * diagonally into the door is lower than the cost of moving on the trap.
				 */
				if ((ds == OPEN_DOOR || branches[current_branch]->map[world->player.status.dungeon][r][c] == OPEN_DOOR) && r != row && c != col)
					continue;
				unsigned int newpathcost = curcost + ((r == row || c == col) ? COST_CARDINAL : COST_DIAGONAL);
				if (s == LAVA)
					newpathcost += COST_LAVA;
				if (s == TRAP)
					newpathcost += COST_TRAP;
				if (s == WATER)
					newpathcost += COST_WATER;
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
	//for (int a = 0; a < 5 && saiph.run(); ++a)
	//	;
	while (saiph.run())
		;
}
