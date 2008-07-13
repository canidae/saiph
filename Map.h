#ifndef MAP_H
/* defines */
#define MAP_H

/* includes */
#include <list>
#include <string>
#include "Globals.h"
#include "Point.h"

/* namespace */
using namespace std;

/* Map */
class Map {
	public:
		/* variables */
		unsigned char dungeon[MAP_ROW_END + 1][MAP_COL_END + 1]; // more "static" features (floors, doors, corridors, ...)
		unsigned char item[MAP_ROW_END + 1][MAP_COL_END + 1]; // items we've seen
		unsigned char monster[MAP_ROW_END + 1][MAP_COL_END + 1]; // monsters we've seen
		list<Point> monsterpos[UCHAR_MAX + 1]; // used to help tracking monsters

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
