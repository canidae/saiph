#ifndef MAP_H
/* defines */
#define MAP_H

/* includes */
#include <string>
#include "Globals.h"

/* namespace */
using namespace std;

/* Map */
class Map {
	public:
		/* variables */
		unsigned char dungeon[ROWS][COLS]; // more "static" features (floors, doors, corridors, ...)
		unsigned char search[ROWS][COLS]; // where we've searched
		unsigned char monster[ROWS][COLS]; // monsters we've seen
		unsigned char item[ROWS][COLS]; // items we've seen

		/* constructors */
		Map();

		/* destructors */
		~Map();

		/* methods */

	private:
		/* variables */

		/* methods */
};
#endif
