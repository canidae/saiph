#ifndef COORDINATE_H
#define COORDINATE_H

#include "Point.h"

class Coordinate : public Point {
public:
	Coordinate(int level = -1, int row = -1, int col = -1);
	Coordinate(int level, const Point& point = Point());

	int level() const;
	int level(int level);

	bool operator<(const Coordinate& c) const;
	bool operator>(const Coordinate& c) const;
	bool operator<=(const Coordinate& c) const;
	bool operator>=(const Coordinate& c) const;
	bool operator==(const Coordinate& c) const;
	bool operator!=(const Coordinate& c) const;

private:
	int _level;
};

std::ostream& operator<<(std::ostream& os, const Coordinate& c);
#endif
