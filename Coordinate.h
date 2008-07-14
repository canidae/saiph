#ifndef COORDINATE_H
/* defines */
#define COORDINATE_H

/* includes */
#include "Point.h"

/* namespace */
using namespace std;

/* Coordinate */
class Coordinate : public Point {
	public:
		/* variables */
		int branch;
		int level;

		/* constructors */
		Coordinate(const Point &point, int branch = 0, int level = 0);
};
#endif
