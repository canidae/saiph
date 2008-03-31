#include "Saiph.h"

/* constructors */
Saiph::Saiph(bool remote) {
	connection = new Connection(remote);
	world = new World(connection);

	/* next command */
	memset(command.command, '\0', MAX_COMMAND_LENGTH);
	command.priority = 0;

	/* pathing */
	pathcost = new unsigned short*[ROWS];
	for (int r = 0; r < ROWS; ++r) {
		pathcost[r] = new unsigned short[COLS];
		for (int c = 0; c < COLS; ++c)
			pathcost[r][c] = 0xffff;
	}
	pathpos = new unsigned char*[MAX_NODES];
	for (int r = 0; r < MAX_NODES; ++r) {
		pathpos[r] = new unsigned char[2];
		pathpos[r][0] = 0;
		pathpos[r][1] = 0;
	}

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
	for (int p = 0; p < MAX_NODES; ++p)
		delete [] pathpos[p];
	delete [] pathpos;
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
			if (branches[current_branch]->search[world->player.status.dungeon][r][c] == 0)
				cout << ' ';
			else
				cout << (char) (branches[current_branch]->search[world->player.status.dungeon][r][c] + 64);
		}
		cout << endl;
	}
	/* world map as the bots sees it */
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
			cout << (char) (pathcost[r][c] + 48);
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

char Saiph::shortestPath(int row, int col, int &distance, bool &direct_line) {
	/* attempt to find the shortest path to destination.
	 * return unsigned character for which direction player should move
	 * or -1 if unable to find a path */
	/* is this dijkstra? */
	distance = 0;
	direct_line = true;
	if (row == world->player.row && col == world->player.col)
		return -1; // path to where we're standing? :o
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c)
			pathcost[r][c] = 0xffff;
	}
	pathcost[row][col] = 0;
	int nextnode = 0;
	int nodes = 1;
	int stopnodes = MAX_NODES - 8; // we check in the while-loop, but increase in the for-loops, may exceed limit
	pathpos[nextnode][0] = row;
	pathpos[nextnode][1] = col;
	int curcost = 0;
	bool target_found = false;
	while (nextnode < nodes && !target_found && nextnode < stopnodes) {
		int sr = pathpos[nextnode][0];
		int sc = pathpos[nextnode][1];
		curcost = pathcost[sr][sc];
		for (int r = sr - 1; r <= sr + 1 && !target_found; ++r) {
			if (r < MAP_ROW_START || r >= MAP_ROW_END)
				continue;
			for (int c = sc - 1; c <= sc + 1 && !target_found; ++c) {
				if (c < 0 || c >= COLS)
					continue;
				char s = branches[current_branch]->map[world->player.status.dungeon][r][c];
				if (s == SOLID_ROCK || s == VERTICAL_WALL || s == HORIZONTAL_WALL || s == CLOSED_DOOR || s == IRON_BARS || s == TREE || s == WATER || s == LAVA || s == BOULDER)
					continue; // can't (or won't) move to these tiles
				if (r != sr && c != sc && (s == OPEN_DOOR || branches[current_branch]->map[world->player.status.dungeon][sr][sc] == OPEN_DOOR))
					continue; // can't move diagonally in/out of a door */
				if (r == world->player.row && c == world->player.col) {
					pathcost[r][c] = curcost + 1;
					target_found = true;
					break;
				}
				int newpathcost = pathcost[sr][sc] + ((r == sr || c == sc) ? COST_CARDINAL : COST_DIAGONAL);
				if (s == TRAP)
					newpathcost = COST_TRAP;
				if (pathcost[r][c] > newpathcost) {
					pathcost[r][c] = newpathcost;
					pathpos[nodes][0] = r;
					pathpos[nodes][1] = c;
					++nodes;
				}
			}
		}
		++nextnode;
	}

	if (!target_found) {
		cerr << "Unable to find path to " << row << ", " << col << " from " << world->player.row << ", " << world->player.col << endl;
		return -1;
	}

	/* least pathcosting move from player is in the right direction */
	int r = world->player.row;
	int c = world->player.col;
	curcost = pathcost[r][c];
	char move = -1;
	char lastmove = -1;
	char firstmove = -1;
	int b = current_branch;
	int d = world->player.status.dungeon;
	while (curcost > 0) {
		++distance;
		int nr = r;
		int nc = c;
		char ss = branches[b]->map[d][r][c];
		if (pathcost[r - 1][c - 1] < curcost && !(ss == OPEN_DOOR || branches[b]->map[d][r - 1][c - 1] == OPEN_DOOR)) {
			move = MOVE_NW;
			nr = r - 1;
			nc = c - 1;
			curcost = pathcost[nr][nc];
		}
		if (pathcost[r - 1][c] < curcost) {
			move = MOVE_N;
			nr = r - 1;
			nc = c;
			curcost = pathcost[nr][nc];
		}
		if (pathcost[r - 1][c + 1] < curcost && !(ss == OPEN_DOOR || branches[b]->map[d][r - 1][c + 1] == OPEN_DOOR)) {
			move = MOVE_NE;
			nr = r - 1;
			nc = c + 1;
			curcost = pathcost[nr][nc];
		}
		if (pathcost[r][c - 1] < curcost) {
			move = MOVE_W;
			nr = r;
			nc = c - 1;
			curcost = pathcost[nr][nc];
		}
		if (pathcost[r][c + 1] < curcost) {
			move = MOVE_E;
			nr = r;
			nc = c + 1;
			curcost = pathcost[nr][nc];
		}
		if (pathcost[r + 1][c - 1] < curcost && !(ss == OPEN_DOOR || branches[b]->map[d][r + 1][c - 1] == OPEN_DOOR)) {
			move = MOVE_SW;
			nr = r + 1;
			nc = c - 1;
			curcost = pathcost[nr][nc];
		}
		if (pathcost[r + 1][c] < curcost) {
			move = MOVE_S;
			nr = r + 1;
			nc = c;
			curcost = pathcost[nr][nc];
		}
		if (pathcost[r + 1][c + 1] < curcost && !(ss == OPEN_DOOR || branches[b]->map[d][r + 1][c + 1] == OPEN_DOOR)) {
			move = MOVE_SE;
			nr = r + 1;
			nc = c + 1;
			curcost = pathcost[nr][nc];
		}
		if (lastmove != -1 && lastmove != move)
			direct_line = false;
		r = nr;
		c = nc;
		lastmove = move;
		if (firstmove == -1)
			firstmove = move;
	}
	return firstmove;
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

/* main */
int main() {
	Saiph saiph(false);
	//for (int a = 0; a < 5 && saiph.run(); ++a)
	//	;
	while (saiph.run())
		;
}
