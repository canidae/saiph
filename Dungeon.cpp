#include "Dungeon.h"

/* constructors */
Dungeon::Dungeon() {
	memset(this->map, ' ', ROWS * (COLS + 1));
	for (int r = 0; r < ROWS; ++r)
		this->map[r][COLS] = '\0';
	memset(this->messages, '\0', BUFFER_SIZE);
}

/* destructors */
Dungeon::~Dungeon() {
}

/* methods */
void Dungeon::clone(const Dungeon *dungeon) {
	for (int r = 0; r < ROWS; ++r)
		strcpy(map[r], dungeon->map[r]);
	strcpy(messages, dungeon->messages);
	player = dungeon->player;
}
