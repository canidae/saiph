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

	/* branches */
	this->current_branch = BRANCH_MAIN;
	for (int b = 0; b < MAX_BRANCHES; ++b) {
		memset(this->branch[b].door, '\0', ROWS * COLS);
		memset(this->branch[b].explore, '\0', ROWS * COLS);
		memset(this->branch[b].search, '\0', ROWS * COLS);
	}

	/* message parser */
	this->parser = new MessageParser(this);

	/* Analyzers */
	this->analyzers = new Analyzer*[MAX_ANALYZERS];
	this->analyzer_count = 0;
	this->analyzers[this->analyzer_count++] = dynamic_cast<Analyzer*>(new DoorAnalyzer(this));
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
void Saiph::farlook(int row, int col) {
	/* row & col is position of monster, not us
	 * that's why we're moving the cursor the opposite way */
	char command[ROWS + COLS + 3];
	int pos = 0;
	command[pos++] = ';';
	while (row > world->player.row && col > world->player.col) {
		command[pos++] = MOVE_SE;
		--row;
		--col;
	}
	while (row > world->player.row && col < world->player.col) {
		command[pos++] = MOVE_SW;
		--row;
		++col;
	}
	while (row < world->player.row && col > world->player.col) {
		command[pos++] = MOVE_NE;
		++row;
		--col;
	}
	while (row < world->player.row && col < world->player.col) {
		command[pos++] = MOVE_NW;
		++row;
		++col;
	}
	while (row > world->player.row) {
		command[pos++] = MOVE_S;
		--row;
	}
	while (row < world->player.row) {
		command[pos++] = MOVE_N;
		++row;
	}
	while (col > world->player.col) {
		command[pos++] = MOVE_E;
		--col;
	}
	while (col < world->player.col) {
		command[pos++] = MOVE_W;
		++col;
	}
	command[pos++] = ',';
	command[pos] = '\0';
	cerr << command << endl;
	world->command(command);
}

bool Saiph::isClosedDoor(char symbol) {
	return (symbol == '7');
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
	return (isObject(symbol) || symbol == ICE || symbol == LOWERED_DRAWBRIDGE || symbol == '#' || symbol == '.' || symbol == '<' || symbol == '>' || symbol == '_' || symbol == '\\' || symbol == '{' || symbol == OPEN_DOOR);
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
	return (isMonster(symbol) || isUnexplored(symbol) || symbol == '|' || symbol == '-' || symbol == '8' || symbol == TREE || symbol == IRON_BARS);
}

void Saiph::parseMessages() {
	cerr << world->messages << endl;
}

bool Saiph::run() {
	/* print stuff so we see what we're doing */
	cout << world->data;
	/* print search map, ish */
	cout << (char) 27 << "[26;1H";
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c) {
			cout << (char) (branch[current_branch].search[r][c] + 48);
		}
		cout << endl;
	}
	for (int r = 0; r < ROWS; ++r) {
		cout << (char) 27 << "[" << r + 1 << ";82H";
		for (int c = 0; c < COLS; ++c) {
			cout << (char) (branch[current_branch].door[r][c] + 48);
		}
	}
	cout << (char) 27 << "[" << world->player.row + 1 << ";" << world->player.col + 1 << "H";

	/* reset command */
	memset(this->command.command, '\0', MAX_COMMAND_LENGTH);
	command.priority = 0;

	/* save dungeon in history */
	++history.map_counter %= HISTORY;
	history.map[history.map_counter].clone(world);

	/* deal with messages */
	parseMessages();

	/* inspect the dungeon */
	inspect();
	//cerr << "Interesting places: " << targets << endl;
	//for (int a = 0; a < targets; ++a) {
	//	cerr << target[a].row << ", " << target[a].col << ": " << target[a].priority << endl;
	//}

	/* call finish() in analyzers */
	for (int a = 0; a < analyzer_count; ++a)
		analyzers[a]->finish();

	/* if no command, quit for now */
	if (command.priority == 0) {
		cout << (char) 27 << "[51;1H";
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

char Saiph::shortestPath(int row, int col) {
	/* attempt to find the shortest path to destination.
	 * return character for which direction player should move
	 * or -1 if unable to find a path */
	/* is this dijkstra? */
	int cost[ROWS][COLS];
	int pos[PATH_MAX_NODES][2];
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c)
			cost[r][c] = PATH_MAX_NODES + 1;
	}
	cost[row][col] = 0;
	int nextnode = 0;
	int nodes = 1;
	pos[nextnode][0] = row;
	pos[nextnode][1] = col;
	int curcost = 0;
	bool target_found = false;
	while (nextnode < nodes && !target_found && nextnode < PATH_MAX_NODES) {
		int tr = pos[nextnode][0];
		int tc = pos[nextnode][1];
		curcost = cost[tr][tc];
		for (int r = tr - 1; r <= tr + 1 && !target_found; ++r) {
			if (r < MAP_ROW_START || r >= MAP_ROW_END)
				continue;
			for (int c = tc - 1; c <= tc + 1 && !target_found; ++c) {
				if (c < 0 || c >= COLS)
					continue;
				char symbol = world->map[r][c];
				if ((branch[current_branch].door[r][c] == DA_DOOR_OPEN || branch[current_branch].door[tr][tc] == DA_DOOR_OPEN) && r != tr && c != tc)
					continue; // can't move diagonally in/out of a door */
				if (r == world->player.row && c == world->player.col) {
					cost[r][c] = curcost + 1;
					target_found = true;
					break;
				}
				if (!isPassable(symbol))
					continue;
				int newcost = cost[tr][tc] + ((r == tr || c == tc) ? 2 : 3);
				if (cost[r][c] > newcost) {
					cost[r][c] = newcost;
					pos[nodes][0] = r;
					pos[nodes][1] = c;
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
	/* print map */
	for (int r = 0; r < ROWS; ++r) {
		cout << (char) 27 << "[" << r + 26 << ";82H";
		for (int c = 0; c < COLS; ++c) {
			cout << (char) (cost[r][c] + 48);
		}
	}
	cout << (char) 27 << "[" << world->player.row + 1 << ";" << world->player.col + 1 << "H";

	cerr << "Pathing to " << row << ", " << col << " from " << world->player.row << ", " << world->player.col << endl;
	/* least costing move from player is in the right direction */
	int r = world->player.row;
	int c = world->player.col;
	curcost = cost[r][c];
	char move = -1;
	char lastmove2 = -1;
	int distance = 0;
	bool direct_line = true;
	while (r != row || c != col) {
		++distance;
		char move2 = -1;
		int nr = r;
		int nc = c;
		if (r > MAP_ROW_START && c > 0 && cost[r - 1][c - 1] < curcost && branch[current_branch].door[r][c] == DA_NO_DOOR && branch[current_branch].door[r - 1][c - 1] == DA_NO_DOOR) {
			move2 = MOVE_NW;
			nr = r - 1;
			nc = c - 1;
			curcost = cost[nr][nc];
		}
		if (r > MAP_ROW_START && cost[r - 1][c] < curcost) {
			move2 = MOVE_N;
			nr = r - 1;
			nc = c;
			curcost = cost[nr][nc];
		}
		if (r > MAP_ROW_START && c < COLS && cost[r - 1][c + 1] < curcost && branch[current_branch].door[r][c] == DA_NO_DOOR && branch[current_branch].door[r - 1][c + 1] == DA_NO_DOOR) {
			move2 = MOVE_NE;
			nr = r - 1;
			nc = c + 1;
			curcost = cost[nr][nc];
		}
		if (c > 0 && cost[r][c - 1] < curcost) {
			move2 = MOVE_W;
			nr = r;
			nc = c - 1;
			curcost = cost[nr][nc];
		}
		if (c < COLS && cost[r][c + 1] < curcost) {
			move2 = MOVE_E;
			nr = r;
			nc = c + 1;
			curcost = cost[nr][nc];
		}
		if (r < MAP_ROW_END && c > 0 && cost[r + 1][c - 1] < curcost && branch[current_branch].door[r][c] == DA_NO_DOOR && branch[current_branch].door[r + 1][c - 1] == DA_NO_DOOR) {
			move2 = MOVE_SW;
			nr = r + 1;
			nc = c - 1;
			curcost = cost[nr][nc];
		}
		if (r < MAP_ROW_END && cost[r + 1][c] < curcost) {
			move2 = MOVE_S;
			nr = r + 1;
			nc = c;
			curcost = cost[nr][nc];
		}
		if (r < MAP_ROW_END && c < COLS && cost[r + 1][c + 1] < curcost && branch[current_branch].door[r][c] == DA_NO_DOOR && branch[current_branch].door[r + 1][c + 1] == DA_NO_DOOR) {
			move2 = MOVE_SE;
			nr = r + 1;
			nc = c + 1;
			curcost = cost[nr][nc];
		}
		if (r == nr && c == nc)
			return -1; // this shouldn' happen
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
	char symbol;
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
				if ((type & analyzers[a]->type) != 0)
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
