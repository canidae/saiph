#include "Level.h"

/* constructors */
Level::Level(string name, unsigned char branch) : name(name), branch(branch) {
	memset(dungeonmap, SOLID_ROCK, sizeof (dungeonmap));
	memset(monstermap, ILLEGAL_MONSTER, sizeof (monstermap));
}
