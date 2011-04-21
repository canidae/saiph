#ifndef COORDINATE_H
#define COORDINATE_H

#include "Point.h"

class Coordinate : public Point {
public:
	Coordinate(int level = -1, int row = -1, int col = -1) : Point(row, col), _level(level) { }
	Coordinate(int level, const Point& point = Point()) : Point(point), _level(level) { }

	int level() const { return _level; }
	int level(int level) { return (_level = level); }

	bool operator<(const Coordinate& c) const;
	bool operator>(const Coordinate& c) const;
	bool operator<=(const Coordinate& c) const;
	bool operator>=(const Coordinate& c) const;
	bool operator==(const Coordinate& c) const { return _level == c._level && row() == c.row() && col() == c.col(); }
	bool operator!=(const Coordinate& c) const { return _level != c._level || row() != c.row() || col() != c.col(); }

private:
	int _level;
};

std::ostream & operator<<(std::ostream& os, const Coordinate& c);
#endif
