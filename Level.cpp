#include "Level.h"

/* constructors */
Level::Level(unsigned char branch) : branch(branch) {
	memset(dungeonmap, SOLID_ROCK, sizeof (dungeonmap));
	memset(monstermap, ILLEGAL_MONSTER, sizeof (monstermap));
}
