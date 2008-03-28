#include "Saiph.h"

/* constructors */
Saiph::Saiph(bool remote) {
	this->connection = new Connection(remote);
	this->world = new World(this->connection);

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

	/* call analyzers */

	/* random, for the time being */
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

	/* do the selected move */
	world->command(command);
}

void Saiph::setNextCommand(const char *command, int priority) {
	/* priority range from 0 to 100.
	 * each analyzer set their own priorities.
	 * this is done so each analyzer may overrule another */
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
			if (symbol == ICE || symbol == LOWERED_DRAWBRIDGE || symbol == '#' || symbol == '.')
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
					analyzers[a]->analyze(r, c);
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
