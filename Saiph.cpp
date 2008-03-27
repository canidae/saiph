#include "Saiph.h"

/* constructors */
Saiph::Saiph(bool remote) {
	this->connection = new Connection(remote);
	this->world = new World(this->connection);

	/* history */
	this->history.map_counter = 0;
	this->history.last_pray = 0;
}

/* destructors */
Saiph::~Saiph() {
	delete world;
	delete connection;
}

/* methods */
void Saiph::run() {
	cout << world->data;
	/* save dungeon in history */
	++history.map_counter %= HISTORY;
	history.map[history.map_counter].clone(world);

	/* deal with messages */
	parseMessages();

	/* inspect the dungeon */
	inspect();

	/* hunger */

	/* threat */

	/* explore */

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

/* private methods */
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

void Saiph::inspect() {
	/* inspect the dungeon for interesting monsters/objects */
	for (int r = MAP_ROW_START; r < MAP_ROW_END; ++r) {
		for (int c = 0; c < COLS; ++c) {
			if (world->map[r][c] == '&' || world->map[r][c] == '\'' || world->map[r][c] == '6' || world->map[r][c] == ':' || world->map[r][c] == ';' || (world->map[r][c] >= '@' && world->map[r][c] <= 'Z') || (world->map[r][c] >= 'a' && world->map[r][c] <= 'z') || world->map[r][c] == '~') {
				/* monster here, look at it */
				farlook(r, c);
				parseMessages();
			} else if (world->map[r][c] == '%') {
				/* there's food or a corpse here, make a special note about it */
			} else if (world->map[r][c] == ')' || world->map[r][c] == '[' || world->map[r][c] == '=' || world->map[r][c] == '"' || world->map[r][c] == '(' || world->map[r][c] == '!' || world->map[r][c] == '?' || world->map[r][c] == '+' || world->map[r][c] == '/' || world->map[r][c] == '$' || world->map[r][c] == '*' || world->map[r][c] == '9') {
				/* interesting object here, make a note of it */
			//} else if (world->map[r][c] == '7' || world->map[r][c] == '<' || world->map[r][c] == '>' || world->map[r][c] == ') {
				/* interesting dungeon feature here, make a note of it */
				/* TODO */
			}
		}
	}
}

void Saiph::parseMessages() {
	cerr << world->messages << endl;
}

/* main */
int main() {
	Saiph saiph(false);
	for (int a = 0; a < 100; ++a)
		saiph.run();
}
