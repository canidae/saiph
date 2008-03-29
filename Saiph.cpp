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
}

/* destructors */
Saiph::~Saiph() {
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
	memset(cost, PATH_MAX_NODES + 1, ROWS * COLS);
	cost[row][col] = 0;
	int nextnode = 0;
	int nodes = 1;
	pos[nextnode][0] = row;
	pos[nextnode][1] = col;
	int curcost = 0;
	bool target_found = false;
	while (curcost++ < PATH_MAX_NODES && !target_found && nextnode < nodes) {
		for (int r = pos[nextnode][0] - 1; r <= pos[nextnode][0] + 1 && !target_found; ++r) {
			if (r < MAP_ROW_START || r >= MAP_ROW_END)
				continue;
			for (int c = pos[nextnode][1] - 1; c <= pos[nextnode][1] + 1 && !target_found; ++col) {
				if (c < 0 || c >= COLS)
					continue;
				char symbol = world->map[r][c];
				if ((r == row && c == col) || (symbol != ICE || symbol != LOWERED_DRAWBRIDGE || symbol != '#' || symbol != '.' || symbol != '<' || symbol != '>' || symbol != '_' || symbol != '\\' || symbol != '{' || symbol != ')' || symbol != '[' || symbol != '=' || symbol != '"' || symbol != '(' || symbol != '%' || symbol != '!' || symbol != '?' || symbol != '+' || symbol != '/' || symbol != '$' || symbol != '*' || symbol != '9'))
					continue;
				if (symbol == '7' && r != row && c != col)
					continue; // can't move diagonally through a door */
				if (cost[r][c] > curcost) {
					cost[r][c] = curcost;
					pos[nodes][0] = r;
					pos[nodes][1] = c;
					++nodes;
					if (r == row && c == col)
						target_found = true;
				}
			}
		}
		++nextnode;
	}
	if (!target_found) {
		cerr << "Unable to find path" << endl;
		return -1;
	}
	/* backtrace */
	int r2 = row;
	int c2 = col;
	curcost = cost[r2][c2];
	cerr << "Path found, " << curcost << " moves needed: ";
	char move = -1;
	while (curcost > 0) {
		if (r2 > MAP_ROW_START && c2 > 0 && cost[r2 - 1][c2 - 1] < curcost) {
			move = MOVE_SE;
			--r2;
			--c2;
		} else if (r2 > MAP_ROW_START && cost[r2 - 1][c2] < curcost) {
			move = MOVE_S;
			--r2;
		} else if (r2 > MAP_ROW_START && c2 < COLS && cost[r2 - 1][c2 + 1] < curcost) {
			move = MOVE_SW;
			--r2;
			++c2;
		} else if (c2 > 0 && cost[r2][c2 - 1] < curcost) {
			move = MOVE_E;
			--c2;
		} else if (c2 < COLS && cost[r2][c2 + 1] < curcost) {
			move = MOVE_W;
			++c2;
		} else if (r2 < MAP_ROW_END && c2 > 0 && cost[r2 + 1][c2 - 1] < curcost) {
			move = MOVE_NE;
			++r2;
			--c2;
		} else if (r2 < MAP_ROW_END && cost[r2 + 1][c2] < curcost) {
			move = MOVE_N;
			++r2;
		} else if (r2 < MAP_ROW_END && c2 < COLS && cost[r2 + 1][c2 + 1] < curcost) {
			move = MOVE_NW;
			++r2;
			++c2;
		}
		curcost = cost[r2][c2];
		cerr << move;
	}
	cerr << endl;
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
			if (symbol == '&' || symbol == '\'' || symbol == '6' || symbol == ':' || symbol == ';' || (symbol >= '@' && symbol <= 'Z') || (symbol >= 'a' && symbol <= 'z') || symbol == '~')
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
			if (symbol == ICE || symbol == LOWERED_DRAWBRIDGE || symbol == '#' || symbol == '.' || symbol == '<' || symbol == '>' || symbol == '_' || symbol == '\\' || symbol == '{')
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
				if (type & analyzers[a]->type != 0)
					analyzers[a]->analyze(r, c, symbol);
			}
		}
	}
}

/* main */
int main() {
	Saiph saiph(false);
	for (int a = 0; a < 100; ++a)
		saiph.run();
}
