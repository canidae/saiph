#include "Map.h"

/* constructors */
Map::Map() {
	memset(dungeon, SOLID_ROCK, sizeof (dungeon));
	memset(monster, ILLEGAL_MONSTER, sizeof (monster));
}
