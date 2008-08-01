#ifndef COORDINATE_H
/* defines */
#define COORDINATE_H

/* forward declare */
class Coordinate;

/* includes */
#include "Point.h"

/* namespace */
using namespace std;

/* Coordinate */
class Coordinate : public Point {
	public:
		/* variables */
		int level;

		/* constructors */
		Coordinate(int level = -1, int row = -1, int col = -1);
		Coordinate(int level, const Point &point = Point());

		/* operator overloading */
		bool operator<(const Coordinate &coordinate) const;
		bool operator>(const Coordinate &coordinate) const;
		bool operator<=(const Coordinate &coordinate) const;
		bool operator>=(const Coordinate &coordinate) const;
		bool operator==(const Coordinate &coordinate) const;
		bool operator!=(const Coordinate &coordinate) const;
};
#endif
