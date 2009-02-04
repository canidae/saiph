#include "Point.h"

/* constructors/destructor */
Point::Point(int row, int col) : row(row), col(col) {
}

/* operator overloading */
bool Point::operator<(const Point &p) const {
	return (row < p.row || (row == p.row && col < p.col));
}

bool Point::operator>(const Point &p) const {
	return (row > p.row || (row == p.row && col > p.col));
}

bool Point::operator<=(const Point &p) const {
	return (row < p.row || (row == p.row && col <= p.col));
}

bool Point::operator>=(const Point &p) const {
	return (row > p.row || (row == p.row && col >= p.col));
}

bool Point::operator==(const Point &p) const {
	return (row == p.row && col == p.col);
}

bool Point::operator!=(const Point &p) const {
	return (row != p.row || col != p.col);
}

std::ostream &operator<<(std::ostream &os, Point &p) {
	os << "(" << p.row << ", " << p.col << ")";
	return os;
}
