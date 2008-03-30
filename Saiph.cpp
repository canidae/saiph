#include "Saiph.h"

/* constructors */
Saiph::Saiph(bool remote) {
	this->connection = new Connection(remote);
	this->world = new World(this->connection);

	/* next command */
	memset(this->command.command, '\0', MAX_COMMAND_LENGTH);
	this->command.priority = 0;

	/* history */
	//memset(this->history.explored, 0, MAX_DUNGEON * ROWS * COLS);
	this->history.map_counter = 0;
	this->history.last_pray = 0;

	/* pathing */
	memset(pathcost, 0xff, ROWS * COLS * sizeof(unsigned short));

	/* branches */
	this->current_branch = BRANCH_MAIN;
	for (int b = 0; b < MAX_BRANCHES; ++b) {
		memset(this->branches[b].map, ' ', MAX_DUNGEON_DEPTH * ROWS * COLS);
		memset(this->branches[b].search, '\0', MAX_DUNGEON_DEPTH * ROWS * COLS);
	}

	/* message parser */
	this->parser = new MessageParser(this);

	/* Analyzers */
	this->analyzers = new Analyzer*[MAX_ANALYZERS];
	this->analyzer_count = 0;
	this->analyzers[this->analyzer_count++] = dynamic_cast<Analyzer*>(new DungeonAnalyzer(this));
	this->analyzers[this->analyzer_count++] = dynamic_cast<Analyzer*>(new ExploreAnalyzer(this));
	this->analyzers[this->analyzer_count++] = dynamic_cast<Analyzer*>(new FightAnalyzer(this));
	this->analyzers[this->analyzer_count++] = dynamic_cast<Analyzer*>(new HealthAnalyzer(this));
}

/* destructors */
Saiph::~Saiph() {
	for (int a = 0; a < analyzer_count; ++a)
		delete analyzers[a];
	delete [] analyzers;
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
			if (branches[current_branch].search[world->player.status.dungeon][r][c] == 0)
				cout << ' ';
			else
				cout << (char) (branches[current_branch].search[world->player.status.dungeon][r][c] + 64);
		}
		cout << endl;
	}
	/* world map as the bots sees it */
	for (int r = 0; r < ROWS; ++r) {
		cout << (char) 27 << "[" << r + 1 << ";82H";
		for (int c = 0; c < COLS; ++c) {
			cout << (char) (branches[current_branch].map[world->player.status.dungeon][r][c]);
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

bool Saiph::hasBoulder(int branch, int dungeon, int row, int col) {
	char symbol = branches[branch].map[dungeon][row][col];
	return isBoulder(symbol);
}

bool Saiph::hasClosedDoor(int branch, int dungeon, int row, int col) {
	char symbol = branches[branch].map[dungeon][row][col];
	return isClosedDoor(symbol);
}

bool Saiph::hasCorridor(int branch, int dungeon, int row, int col) {
	char symbol = branches[branch].map[dungeon][row][col];
	return isCorridor(symbol);
}

bool Saiph::hasMonster(int branch, int dungeon, int row, int col) {
	/* FIXME
	 * should use another map for tracking monsters */
	char symbol = world->map[row][col];
	return isMonster(symbol);
}

bool Saiph::hasObject(int branch, int dungeon, int row, int col) {
	/* FIXME
	 * should use another map for tracking objects */
	char symbol = world->map[row][col];
	return isObject(symbol);
}

bool Saiph::hasOpenDoor(int branch, int dungeon, int row, int col) {
	char symbol = branches[branch].map[dungeon][row][col];
	return isOpenDoor(symbol);
}

bool Saiph::hasPassable(int branch, int dungeon, int row, int col) {
	char symbol = branches[branch].map[dungeon][row][col];
	return isPassable(symbol);
}

bool Saiph::hasPet(int branch, int dungeon, int row, int col) {
	/* FIXME
	 * should use another map for tracking pets */
	char symbol = world->map[row][col];
	return isPet(symbol);
}

bool Saiph::hasPlayer(int branch, int dungeon, int row, int col) {
	/* since there's only 1 player this is identical to isPlayer() */
	char symbol = world->map[row][col];
	return isPlayer(symbol);
}

bool Saiph::hasTrap(int branch, int dungeon, int row, int col) {
	char symbol = branches[branch].map[dungeon][row][col];
	return isTrap(symbol);
}

bool Saiph::hasUnexplored(int branch, int dungeon, int row, int col) {
	char symbol = branches[branch].map[dungeon][row][col];
	return isUnexplored(symbol);
}

bool Saiph::hasUnpassable(int branch, int dungeon, int row, int col) {
	char symbol = branches[branch].map[dungeon][row][col];
	return isUnpassable(symbol);
}

bool Saiph::isBoulder(char symbol) {
	return (symbol == '0');
}

bool Saiph::isClosedDoor(char symbol) {
	return (symbol == '7');
}

bool Saiph::isCorridor(char symbol) {
	return (symbol == '#');
}

bool Saiph::isMonster(char symbol) {
	return (symbol == '&' || symbol == '\'' || symbol == '6' || symbol == ':' || symbol == ';' || (symbol >= '@' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z') || symbol == '~');
}

bool Saiph::isObject(char symbol) {
	return (symbol == ')' || symbol == '[' || symbol == '=' || symbol == '"' || symbol == '(' || symbol == '%' || symbol == '!' || symbol == '?' || symbol == '+' || symbol == '/' || symbol == '$' || symbol == '*' || symbol == '9');
}

bool Saiph::isOpenDoor(char symbol) {
	return (symbol == OPEN_DOOR);
}

bool Saiph::isPassable(char symbol) {
	return (symbol == ICE || symbol == LOWERED_DRAWBRIDGE || symbol == '#' || symbol == '.' || symbol == '<' || symbol == '>' || symbol == '_' || symbol == '\\' || symbol == '{' || symbol == OPEN_DOOR);
}

bool Saiph::isPet(char symbol) {
	return (symbol == PET);
}

bool Saiph::isPlayer(char symbol) {
	return (symbol == PLAYER);
}

bool Saiph::isTrap(char symbol) {
	return (symbol == '^');
}

bool Saiph::isUnexplored(char symbol) {
	return (symbol == ' ');
}

bool Saiph::isUnpassable(char symbol) {
	return (symbol == '|' || symbol == '-' || symbol == '8' || symbol == TREE || symbol == IRON_BARS);
}

bool Saiph::run() {
	/* print stuff so we see what we're doing */
	dumpScreens();

	/* reset command */
	memset(this->command.command, '\0', MAX_COMMAND_LENGTH);
	command.priority = 0;

	/* save dungeon in history */
	++history.map_counter %= HISTORY;
	history.map[history.map_counter].clone(world);

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
	if (priority < this->command.priority)
		return;
	strcpy(this->command.command, command);
	this->command.priority = priority;
}

char Saiph::shortestPath(int branch, int dungeon, int row, int col, int &distance, bool &direct_line) {
	/* attempt to find the shortest path to destination.
	 * return unsigned character for which direction player should move
	 * or -1 if unable to find a path */
	/* is this dijkstra? */
	distance = 0;
	direct_line = true;
	memset(pathcost, 0xff, ROWS * COLS * sizeof(unsigned short));
	pathcost[row][col] = 0;
	int nextnode = 0;
	int nodes = 1;
	pathpos[nextnode][0] = row;
	pathpos[nextnode][1] = col;
	int curcost = 0;
	bool target_found = false;
	while (nextnode < nodes && !target_found && nextnode < MAX_NODES) {
		int tr = pathpos[nextnode][0];
		int tc = pathpos[nextnode][1];
		curcost = pathcost[tr][tc];
		for (int r = tr - 1; r <= tr + 1 && !target_found; ++r) {
			if (r < MAP_ROW_START || r >= MAP_ROW_END)
				continue;
			for (int c = tc - 1; c <= tc + 1 && !target_found; ++c) {
				if (c < 0 || c >= COLS)
					continue;
				if (r != tr && c != tc && (hasOpenDoor(branch, dungeon, r, c) || hasOpenDoor(branch, dungeon, tr, tc)))
					continue; // can't move diagonally in/out of a door */
				if (!hasPassable(branch, dungeon, r, c) || hasClosedDoor(branch, dungeon, r, c))
					continue;
				if (r == world->player.row && c == world->player.col) {
					pathcost[r][c] = curcost + 1;
					target_found = true;
					break;
				}
				int newpathcost = pathcost[tr][tc] + ((r == tr || c == tc) ? COST_CARDINAL : COST_DIAGONAL);
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
	char lastmove2 = -1;
	while (r != row || c != col) {
		++distance;
		unsigned char move2 = -1;
		int nr = r;
		int nc = c;
		if (r > MAP_ROW_START && c > 0 && pathcost[r - 1][c - 1] < curcost && !hasOpenDoor(branch, dungeon, r, c) && !hasOpenDoor(branch, dungeon, r - 1, c - 1)) {
			move2 = MOVE_NW;
			nr = r - 1;
			nc = c - 1;
			curcost = pathcost[nr][nc];
		}
		if (r > MAP_ROW_START && pathcost[r - 1][c] < curcost) {
			move2 = MOVE_N;
			nr = r - 1;
			nc = c;
			curcost = pathcost[nr][nc];
		}
		if (r > MAP_ROW_START && c < COLS && pathcost[r - 1][c + 1] < curcost && !hasOpenDoor(branch, dungeon, r, c) && !hasOpenDoor(branch, dungeon, r - 1, c + 1)) {
			move2 = MOVE_NE;
			nr = r - 1;
			nc = c + 1;
			curcost = pathcost[nr][nc];
		}
		if (c > 0 && pathcost[r][c - 1] < curcost) {
			move2 = MOVE_W;
			nr = r;
			nc = c - 1;
			curcost = pathcost[nr][nc];
		}
		if (c < COLS && pathcost[r][c + 1] < curcost) {
			move2 = MOVE_E;
			nr = r;
			nc = c + 1;
			curcost = pathcost[nr][nc];
		}
		if (r < MAP_ROW_END && c > 0 && pathcost[r + 1][c - 1] < curcost && !hasOpenDoor(branch, dungeon, r, c) && !hasOpenDoor(branch, dungeon, r + 1, c - 1)) {
			move2 = MOVE_SW;
			nr = r + 1;
			nc = c - 1;
			curcost = pathcost[nr][nc];
		}
		if (r < MAP_ROW_END && pathcost[r + 1][c] < curcost) {
			move2 = MOVE_S;
			nr = r + 1;
			nc = c;
			curcost = pathcost[nr][nc];
		}
		if (r < MAP_ROW_END && c < COLS && pathcost[r + 1][c + 1] < curcost && !hasOpenDoor(branch, dungeon, r, c) && !hasOpenDoor(branch, dungeon, r + 1, c + 1)) {
			move2 = MOVE_SE;
			nr = r + 1;
			nc = c + 1;
			curcost = pathcost[nr][nc];
		}
		if (r == nr && c == nc) {
			//if (curcost > COST_DIAGONAL || curcost > COST_CARDINAL) {
				cerr << "THIS SHOULDN'T HAPPEN, SO WHY THE FUCK DOES IT?" << endl;
				sleep(1000);
				exit(99);
				return -1; // this shouldn' happen
			//}
			//return lastmove2;
		}
		if (move == -1)
			move = move2;
		if (lastmove2 != -1 && lastmove2 != move2)
			direct_line = false;
		r = nr;
		c = nc;
		lastmove2 = move2;
	}
	return move;
}

/* private methods */
void Saiph::inspect() {
	/* inspect the dungeon for interesting monsters/objects/places */
	//targets = 0;
	unsigned char symbol;
	for (int r = MAP_ROW_START; r <= MAP_ROW_END; ++r) {
		for (int c = 0; c < COLS; ++c) {
			int type = 0;
			symbol = world->map[r][c];
			type |= isClosedDoor(symbol) ? ANALYZE_CLOSED_DOOR : ANALYZE_NONE;
			type |= isMonster(symbol) ? ANALYZE_MONSTER : ANALYZE_NONE;
			type |= isObject(symbol) ? ANALYZE_OBJECT : ANALYZE_NONE;
			type |= isOpenDoor(symbol) ? ANALYZE_OPEN_DOOR : ANALYZE_NONE;
			type |= isPassable(symbol) ? ANALYZE_PASSABLE : ANALYZE_NONE;
			type |= isPet(symbol) ? ANALYZE_PET : ANALYZE_NONE;
			type |= isPlayer(symbol) ? ANALYZE_PLAYER : ANALYZE_NONE;
			type |= isTrap(symbol) ? ANALYZE_TRAP : ANALYZE_NONE;
			type |= isUnpassable(symbol) ? ANALYZE_UNPASSABLE : ANALYZE_NONE;
			for (int a = 0; a < analyzer_count; ++a) {
				if ((type & analyzers[a]->type) != 0 || analyzers[a]->type == ANALYZE_ALL)
					analyzers[a]->analyze(r, c, symbol);
			}
		}
	}
}

/* main */
int main() {
	Saiph saiph(false);
	while (saiph.run())
		;
}
