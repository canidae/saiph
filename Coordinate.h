#ifndef COORDINATE_H
#define COORDINATE_H

#include "Point.h"

class Coordinate : public Point {
	public:
		int level;

		Coordinate(int level = -1, int row = -1, int col = -1);
		Coordinate(int level, const Point &point = Point());

		bool operator<(const Coordinate &coordinate) const;
		bool operator>(const Coordinate &coordinate) const;
		bool operator<=(const Coordinate &coordinate) const;
		bool operator>=(const Coordinate &coordinate) const;
		bool operator==(const Coordinate &coordinate) const;
		bool operator!=(const Coordinate &coordinate) const;
};
#endif
