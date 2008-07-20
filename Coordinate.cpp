#include "Coordinate.h"

/* constructors */
Coordinate::Coordinate(int branch, int level, int row, int col) : Point(row, col), branch(branch), level(level) {
}

Coordinate::Coordinate(int branch, int level, const Point &point) : Point(point), branch(branch), level(level) {
}

/* operator overloading */
bool Coordinate::operator<(const Coordinate &coordinate) const {
	return (coordinate.branch < branch || (coordinate.branch == branch && coordinate.level < level) || (coordinate.branch == branch && coordinate.level == level && coordinate.row < row) || (coordinate.branch == branch && coordinate.level == level && coordinate.row == row && coordinate.col < col));
}

bool Coordinate::operator>(const Coordinate &coordinate) const {
	return (coordinate.branch > branch || (coordinate.branch == branch && coordinate.level > level) || (coordinate.branch == branch && coordinate.level == level && coordinate.row > row) || (coordinate.branch == branch && coordinate.level == level && coordinate.row == row && coordinate.col > col));
}

bool Coordinate::operator<=(const Coordinate &coordinate) const {
	return (coordinate.branch < branch || (coordinate.branch == branch && coordinate.level < level) || (coordinate.branch == branch && coordinate.level == level && coordinate.row < row) || (coordinate.branch == branch && coordinate.level == level && coordinate.row == row && coordinate.col <= col));
}

bool Coordinate::operator>=(const Coordinate &coordinate) const {
	return (coordinate.branch > branch || (coordinate.branch == branch && coordinate.level > level) || (coordinate.branch == branch && coordinate.level == level && coordinate.row > row) || (coordinate.branch == branch && coordinate.level == level && coordinate.row == row && coordinate.col >= col));
}

bool Coordinate::operator==(const Coordinate &coordinate) const {
	return (coordinate.branch == branch && coordinate.level == level && coordinate.row == row && coordinate.col == col);
}

bool Coordinate::operator!=(const Coordinate &coordinate) const {
	return (coordinate.branch != branch || coordinate.level != level || coordinate.row != row || coordinate.col != col);
}
