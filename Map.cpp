#include "Map.h"

/* constructors */
Map::Map() {
	memset(dungeon, SOLID_ROCK, sizeof (dungeon));
	memset(item, NOITEM, sizeof (item));
	memset(monster, NOMONSTER, sizeof (monster));
}

/* destructors */
Map::~Map() {
}
