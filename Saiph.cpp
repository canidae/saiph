#include "Saiph.h"

/* constructors */
Saiph::Saiph(bool remote) {
	this->connection = new Connection(remote);
	this->world = new World(this->connection);

	/* next command */
	memset(this->command.command, '\0', MAX_COMMAND_LENGTH);
	this->command.priority = 0;

	/* history */
	memset(this->history.search, 0, MAX_DUNGEON * ROWS * COLS);
	this->history.map_counter = 0;
	this->history.last_pray = 0;

	/* message parser */
	this->parser = new MessageParser(this);

	/* Analyzers */
	this->analyzers = new Analyzer*[MAX_ANALYZERS];
	this->analyzer_count = 0;
	this->analyzers[this->analyzer_count++] = dynamic_cast<Analyzer*>(new HealthAnalyzer(this));
	this->analyzers[this->analyzer_count++] = dynamic_cast<Analyzer*>(new FightAnalyzer(this));
}

/* destructors */
Saiph::~Saiph() {
	delete [] analyzers;
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

void Saiph::parseMessages() {
	cerr << world->messages << endl;
}

void Saiph::run() {
	cout << world->data;
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

	/* random, for the time being */
	/*
	char command[2];
	switch (rand() % 8) {
		case 0:
			command[0] = MOVE_NW;
			break;

		case 1:
			command[0] = MOVE_N;
			break;

		case 2:
			command[0] = MOVE_NE;
			break;

		case 3:
			command[0] = MOVE_W;
			break;

		case 4:
			command[0] = MOVE_E;
			break;

		case 5:
			command[0] = MOVE_SW;
			break;

		case 6:
			command[0] = MOVE_S;
			break;

		case 7:
			command[0] = MOVE_SE;
			break;
	}
	command[1] = '\0';
	*/

	/* if no command, search */
	setNextCommand("s", 0);

	/* do the selected move */
	cerr << "Command: " << command.command << " (priority: " << command.priority << ")" << endl;
	world->command(command.command);
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
				if (r == world->player.row && c == world->player.col) {
					cost[r][c] = curcost + 1;
					target_found = true;
					break;
				}
				char symbol = world->map[r][c];
				if (!isPassable(symbol))
					continue;
				if (symbol == '7' && r != row && c != col)
					continue; // can't move diagonally through a door */
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

	/* print map cost, ish */
	cout << (char) 27 << "[26;1H";
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS; ++c) {
			cout << (char) (cost[r][c] + 64);
		}
		cout << endl;
	}
	if (!target_found) {
		cerr << "Unable to find path" << endl;
		return -1;
	}
	/* least costing move from player is in the right direction */
	int r = world->player.row;
	int c = world->player.col;
	curcost = cost[r][c];
	char move = -1;
	char lastmove2 = -1;
	int distance = 0;
	bool direct_line = true;
	cerr << "Going from " << r << ", " << c << " to " << row << ", " << col << endl;
	cerr << "Path found, moves needed: " << endl;
	while (r != row || c != col) {
		++distance;
		char move2 = -1;
		int nr = r;
		int nc = c;
		cerr << r << ", " << c << " | " << row << ", " << col << endl;
		if (r > MAP_ROW_START && c > 0 && cost[r - 1][c - 1] < curcost) {
			cerr << "- " << cost[r - 1][c - 1] << " | " << curcost << endl;
			move2 = MOVE_NW;
			nr = r - 1;
			nc = c - 1;
			curcost = cost[nr][nc];
		}
		if (r > MAP_ROW_START && cost[r - 1][c] < curcost) {
			cerr << "- " << cost[r - 1][c] << " | " << curcost << endl;
			move2 = MOVE_N;
			nr = r - 1;
			nc = c;
			curcost = cost[nr][nc];
		}
		if (r > MAP_ROW_START && c < COLS && cost[r - 1][c + 1] < curcost) {
			cerr << "- " << cost[r - 1][c + 1] << " | " << curcost << endl;
			move2 = MOVE_NE;
			nr = r - 1;
			nc = c + 1;
			curcost = cost[nr][nc];
		}
		if (c > 0 && cost[r][c - 1] < curcost) {
			cerr << "- " << cost[r][c - 1] << " | " << curcost << endl;
			move2 = MOVE_W;
			nr = r;
			nc = c - 1;
			curcost = cost[nr][nc];
		}
		if (c < COLS && cost[r][c + 1] < curcost) {
			cerr << "- " << cost[r][c + 1] << " | " << curcost << endl;
			move2 = MOVE_E;
			nr = r;
			nc = c + 1;
			curcost = cost[nr][nc];
		}
		if (r < MAP_ROW_END && c > 0 && cost[r + 1][c - 1] < curcost) {
			cerr << "- " << cost[r + 1][c - 1] << " | " << curcost << endl;
			move2 = MOVE_SW;
			nr = r + 1;
			nc = c - 1;
			curcost = cost[nr][nc];
		}
		if (r < MAP_ROW_END && cost[r + 1][c] < curcost) {
			cerr << "- " << cost[r + 1][c] << " | " << curcost << endl;
			move2 = MOVE_S;
			nr = r + 1;
			nc = c;
			curcost = cost[nr][nc];
		}
		if (r < MAP_ROW_END && c < COLS && cost[r + 1][c + 1] < curcost) {
			cerr << "- " << cost[r + 1][c + 1] << " | " << curcost << endl;
			move2 = MOVE_SE;
			nr = r + 1;
			nc = c + 1;
			curcost = cost[nr][nc];
		}
		if (move == -1)
			move = move2;
		if (lastmove2 != -1 && lastmove2 != move2)
			direct_line = false;
		r = nr;
		c = nc;
		lastmove2 = move2;
		cerr << move2 << endl;;
	}
	cerr << endl;
	cerr << "Direct line: " << direct_line << endl;
	cerr << "Distance: " << distance << endl;

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
			if (isMonster(symbol))
				type |= ANALYZE_MONSTER;
			if (symbol == ')' || symbol == '[' || symbol == '=' || symbol == '"' || symbol == '(' || symbol == '%' || symbol == '!' || symbol == '?' || symbol == '+' || symbol == '/' || symbol == '$' || symbol == '*' || symbol == '9')
				type |= ANALYZE_OBJECT;
			/*
				if (history.search[world->player.status.dungeon][r][c] < MAX_SEARCH) {
					target[targets].row = r;
					target[targets].col = c;
					target[targets].priority = 51;
					++targets;
				}
			*/
			if (symbol == OPEN_DOOR || symbol == '7')
				type |= ANALYZE_DOOR;
			/*
				if (history.search[world->player.status.dungeon][r][c] < MAX_SEARCH && (world->map[r][c - 1] == ' ' || world->map[r][c + 1] == ' ' || world->map[r - 1][c] == ' ' || world->map[r + 1][c] == ' ')) {
					target[targets].row = r;
					target[targets].col = c;
					target[targets].priority = 50;
					++targets;
				}
			*/
			if (isPassable(symbol))
				type |= ANALYZE_PASSABLE;
			/*
				if (history.search[world->player.status.dungeon][r][c] >= MAX_SEARCH || r <= MAP_ROW_START + 1 || r >= MAP_ROW_END - 1 || c <= 1 || c >= COLS - 1)
					continue;
				if (((world->map[r][c - 1] == ' ' || world->map[r][c + 1] == ' ') && ((world->map[r - 1][c] == '|' || world->map[r - 1][c] == '-') && (world->map[r + 1][c] == '|' || world->map[r + 1][c] == '-'))) || ((world->map[r - 1][c] == ' ' || world->map[r + 1][c] == ' ') && ((world->map[r][c - 1] == '|' || world->map[r][c - 1] == '-') && (world->map[r][c + 1] == '|' || world->map[r][c + 1] == '-')))) {
					// missing door
					target[targets].row = r;
					target[targets].col = c;
					target[targets].priority = 40;
					++targets;
					continue;
				}
				int paths = 0;
				for (int r2 = r - 1; r2 <= r + 1; ++r2) {
					for (int c2 = c - 1; c2 <= c + 1; ++c2) {
						if (r2 == r && c2 == c)
							continue;
						if (world->map[r2][c2] != ' ' && world->map[r2][c2] != '-' && world->map[r2][c2] != '|' && world->map[r2][c2] != '8' && world->map[r2][c2] != '}' && world->map[r2][c2] != TREE && world->map[r2][c2] != IRON_BARS)
							++paths;
					}
				}
				if (paths <= 1) {
					target[targets].row = r;
					target[targets].col = c;
					target[targets].priority = 20;
					++targets;
				}
			*/
			if (symbol == '^')
				type |= ANALYZE_TRAP;
			for (int a = 0; a < analyzer_count; ++a) {
				if ((type & analyzers[a]->type) != 0)
					analyzers[a]->analyze(r, c, symbol);
			}
		}
	}
}

bool Saiph::isMonster(char symbol) {
	return (symbol == '&' || symbol == '\'' || symbol == '6' || symbol == ':' || symbol == ';' || (symbol >= '@' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z') || symbol == '~');
}

bool Saiph::isPassable(char symbol) {
	return (symbol == ICE || symbol == LOWERED_DRAWBRIDGE || symbol == '#' || symbol == '.' || symbol == '<' || symbol == '>' || symbol == '_' || symbol == '\\' || symbol == '{' || symbol == ')' || symbol == '[' || symbol == '=' || symbol == '"' || symbol == '(' || symbol == '%' || symbol == '!' || symbol == '?' || symbol == '+' || symbol == '/' || symbol == '$' || symbol == '*' || symbol == '9' || symbol == OPEN_DOOR);
}

/* main */
int main() {
	Saiph saiph(false);
	for (int a = 0; a < 100; ++a)
		saiph.run();
}
