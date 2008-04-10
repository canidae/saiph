#include "Dungeon.h"

/* constructors */
Dungeon::Dungeon() {
	memset(map, ' ', ROWS * (COLS + 1));
	for (int r = 0; r < ROWS; ++r)
		map[r][COLS] = '\0';
	memset(colour, 0, ROWS * COLS);
	memset(messages, '\0', BUFFER_SIZE);
}

/* methods */
void Dungeon::clone(const Dungeon *dungeon) {
	for (int r = 0; r < ROWS; ++r) {
		for (int c = 0; c < COLS + 1; ++c) {
			map[r][c] = dungeon->map[r][c];
			colour[r][c] = dungeon->colour[r][c];
		}
		map[r][80] = dungeon->map[r][80];
	}
	strcpy(messages, dungeon->messages);
	player = dungeon->player;
}
