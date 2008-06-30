#include "Map.h"

/* constructors */
Map::Map() {
	memset(dungeon, SOLID_ROCK, ROWS * COLS);
	memset(search, 0, ROWS * COLS);
	memset(monster, NOMONSTER, ROWS * COLS);
	memset(item, NOITEM, ROWS * COLS);
}

/* destructors */
Map::~Map() {
}
