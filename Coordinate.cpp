#include "Coordinate.h"

/* constructors/destructor */
Coordinate::Coordinate(int level, int row, int col) : Point(row, col), level(level) {
}

Coordinate::Coordinate(int level, const Point &point) : Point(point), level(level) {
}

/* operator overloading */
bool Coordinate::operator<(const Coordinate &coordinate) const {
	return (level < coordinate.level || (level == coordinate.level && row < coordinate.row) || (level == coordinate.level && row == coordinate.row && col < coordinate.col));
}

bool Coordinate::operator>(const Coordinate &coordinate) const {
	return (level > coordinate.level || (level == coordinate.level && row > coordinate.row) || (level == coordinate.level && row == coordinate.row && col > coordinate.col));
}

bool Coordinate::operator<=(const Coordinate &coordinate) const {
	return (level < coordinate.level || (level == coordinate.level && row < coordinate.row) || (level == coordinate.level && row == coordinate.row && col <= coordinate.col));
}

bool Coordinate::operator>=(const Coordinate &coordinate) const {
	return (level > coordinate.level || (level == coordinate.level && row > coordinate.row) || (level == coordinate.level && row == coordinate.row && col >= coordinate.col));
}

bool Coordinate::operator==(const Coordinate &coordinate) const {
	return (level == coordinate.level && row == coordinate.row && col == coordinate.col);
}

bool Coordinate::operator!=(const Coordinate &coordinate) const {
	return (level != coordinate.level || row != coordinate.row || col != coordinate.col);
}
