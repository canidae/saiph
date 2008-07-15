#ifndef MAP_H
/* defines */
#define MAP_H

/* includes */
#include <string>
#include "Globals.h"

/* namespaces */
using namespace std;

/* Map */
class Map {
	public:
		/* variables */
		unsigned char dungeon[MAP_ROW_END + 1][MAP_COL_END + 1]; // more "static" features (floors, doors, corridors, ...)
		unsigned char monster[MAP_ROW_END + 1][MAP_COL_END + 1]; // monsters we've seen

		/* constructors */
		Map();
};
#endif
