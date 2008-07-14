#ifndef MONSTER_H
/* defines */
#define MONSTER_H

/* includes */
#include "Point.h"

/* namespaces */
using namespace std;

/* Monster */
class Monster : public Point {
	public:
		/* variables */
		unsigned char symbol;
		int color;
};
#endif
