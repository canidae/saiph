#ifndef DUNGEON_H
/* defines */
#define DUNGEON_H
/* buffer for various stuff */
#define BUFFER_SIZE 16384
/* dungeon limits */
#define ROWS 24
#define COLS 80

/* forward declare */
class Dungeon;

/* includes */
#include "Player.h"

/* namespace */
using namespace std;

/* a dungeon represents a map, including attribute/status rows */
class Dungeon {
	public:
		/* variables */
		char map[ROWS][COLS + 1];
		char messages[BUFFER_SIZE];
		Player player;

		/* constructors */
		Dungeon();

		/* destructors */
		~Dungeon();

		/* methods */
		void clone(const Dungeon *dungeon);
};

#endif
