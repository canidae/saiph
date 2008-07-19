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
		Coordinate(int branch = 0, int level = 0, int row = 0, int col = 0);
		Coordinate(int branch, int level, const Point &point = Point());
};
#endif
