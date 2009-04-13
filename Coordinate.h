#ifndef COORDINATE_H
#define COORDINATE_H

#include "Point.h"

class Coordinate : public Point {
public:
	int level;

	Coordinate(int level = -1, int row = -1, int col = -1) : Point(row, col), level(level) {}
	Coordinate(int level, const Point &point = Point()) : Point(point), level(level) {}

	bool operator<(const Coordinate &c) const {return (level < c.level || (level == c.level && row < c.row) || (level == c.level && row == c.row && col < c.col));}
	bool operator>(const Coordinate &c) const {return (level > c.level || (level == c.level && row > c.row) || (level == c.level && row == c.row && col > c.col));}
	bool operator<=(const Coordinate &c) const {return (level < c.level || (level == c.level && row < c.row) || (level == c.level && row == c.row && col <= c.col));}
	bool operator>=(const Coordinate &c) const {return (level > c.level || (level == c.level && row > c.row) || (level == c.level && row == c.row && col >= c.col));}
	bool operator==(const Coordinate &c) const {return (level == c.level && row == c.row && col == c.col);}
	bool operator!=(const Coordinate &c) const {return (level != c.level || row != c.row || col != c.col);}
};

std::ostream &operator<<(std::ostream &os, const Coordinate& c) {return os << "(" << c.level << ", " << c.row << ", " << c.col << ")";}
#endif
