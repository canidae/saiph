#ifndef MONSTER_H
/* defines */
#define MONSTER_H

/* includes */
#include "Globals.h"
#include "Point.h"

/* namespaces */
using namespace std;

/* Monster */
class Monster : public Point {
	public:
		/* variables */
		unsigned char symbol;
		int color;

		/* constructors */
		Monster(const Point &point = Point(), unsigned char symbol = ILLEGAL_MONSTER, int color = 0);
};
#endif
