#include "Coordinate.h"

/* constructors/destructor */
Coordinate::Coordinate(int level, int row, int col) : Point(row, col), level(level) {
}

Coordinate::Coordinate(int level, const Point &point) : Point(point), level(level) {
}

/* operator overloading */
bool Coordinate::operator<(const Coordinate &c) const {
	return (level < c.level || (level == c.level && row < c.row) || (level == c.level && row == c.row && col < c.col));
}

bool Coordinate::operator>(const Coordinate &c) const {
	return (level > c.level || (level == c.level && row > c.row) || (level == c.level && row == c.row && col > c.col));
}

bool Coordinate::operator<=(const Coordinate &c) const {
	return (level < c.level || (level == c.level && row < c.row) || (level == c.level && row == c.row && col <= c.col));
}

bool Coordinate::operator>=(const Coordinate &c) const {
	return (level > c.level || (level == c.level && row > c.row) || (level == c.level && row == c.row && col >= c.col));
}

bool Coordinate::operator==(const Coordinate &c) const {
	return (level == c.level && row == c.row && col == c.col);
}

bool Coordinate::operator!=(const Coordinate &c) const {
	return (level != c.level || row != c.row || col != c.col);
}

std::ostream &operator<<(std::ostream &os, const Coordinate &c) {
	return os << "(" << c.level << ", " << c.row << ", " << c.col << ")";
}
