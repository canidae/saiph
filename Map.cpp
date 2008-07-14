#include "Map.h"

/* constructors */
Map::Map() {
	memset(dungeon, SOLID_ROCK, sizeof (dungeon));
	memset(item, ILLEGAL_ITEM, sizeof (item));
	memset(monster, ILLEGAL_MONSTER, sizeof (monster));
}
