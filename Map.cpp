#include "Map.h"

/* constructors */
Map::Map() {
	memset(dungeon, SOLID_ROCK, sizeof(dungeon));
	memset(search, 0, sizeof(search));
	memset(monster, NOMONSTER, sizeof(monster));
	memset(item, NOITEM, sizeof(item));
}

/* destructors */
Map::~Map() {
}
