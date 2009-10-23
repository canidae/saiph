#ifndef COORDINATE_H
#define COORDINATE_H

#include "Point.h"

class Coordinate : public Point {
public:
	Coordinate(const int& level = -1, const int& row = -1, const int& col = -1);
	Coordinate(const int& level, const Point& point = Point());

	const int& level() const;
	const int& level(const int& level);

	bool operator<(const Coordinate& c) const;
	bool operator>(const Coordinate& c) const;
	bool operator<=(const Coordinate& c) const;
	bool operator>=(const Coordinate& c) const;
	bool operator==(const Coordinate& c) const;
	bool operator!=(const Coordinate& c) const;

private:
	int _level;
};

std::ostream & operator<<(std::ostream& os, const Coordinate& c);
#endif
