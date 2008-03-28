#ifndef DUNGEON_H
#define DUNGEON_H

/* forward declare */
class Dungeon;

#include "Globals.h"
#include "Player.h"

using namespace std;

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
