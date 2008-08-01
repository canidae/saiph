#ifndef LEVEL_H
/* defines */
#define LEVEL_H

/* forward declare */
class Level;

/* includes */
#include <map>
#include "Globals.h"
#include "Monster.h"
#include "Point.h"
#include "Stash.h"

/* namespaces */
using namespace std;

/* Level */
class Level {
	public:
		/* variables */
		unsigned char dungeonmap[MAP_ROW_END + 1][MAP_COL_END + 1];
		unsigned char monstermap[MAP_ROW_END + 1][MAP_COL_END + 1];
		map<Point, Monster> monsters;
		map<Point, Stash> stashes;
		map<unsigned char, map<Point, int> > symbols;
		unsigned char branch;

		/* constructors */
		Level(unsigned char branch = BRANCH_MAIN);
};
#endif
