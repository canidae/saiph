#include "Point.h"

/* constructors/destructor */
Point::Point(int row, int col) : row(row), col(col) {
}

/* operator overloading */
bool Point::operator<(const Point &point) const {
	return (point.row < row || (point.row == row && point.col < col));
}

bool Point::operator>(const Point &point) const {
	return (point.row > row || (point.row == row && point.col > col));
}

bool Point::operator<=(const Point &point) const {
	return (point.row < row || (point.row == row && point.col <= col));
}

bool Point::operator>=(const Point &point) const {
	return (point.row > row || (point.row == row && point.col >= col));
}

bool Point::operator==(const Point &point) const {
	return (point.row == row && point.col == col);
}

bool Point::operator!=(const Point &point) const {
	return (point.row != row || point.col != col);
}
