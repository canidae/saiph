#include "Coordinate.h"

using namespace std;

/* constructors/destructors */

/* methods */

/* operator overloading */
bool Coordinate::operator<(const Coordinate& c) const {
	return (_level < c._level || (_level == c._level && row() < c.row()) || (_level == c._level && row() == c.row() && col() < c.col()));
}

bool Coordinate::operator>(const Coordinate& c) const {
	return (_level > c._level || (_level == c._level && row() > c.row()) || (_level == c._level && row() == c.row() && col() > c.col()));
}

bool Coordinate::operator<=(const Coordinate& c) const {
	return (_level < c._level || (_level == c._level && row() < c.row()) || (_level == c._level && row() == c.row() && col() <= c.col()));
}

bool Coordinate::operator>=(const Coordinate& c) const {
	return (_level > c._level || (_level == c._level && row() > c.row()) || (_level == c._level && row() == c.row() && col() >= c.col()));
}

ostream & operator<<(ostream& os, const Coordinate& c) {
	return os << "(" << c.level() << ", " << c.row() << ", " << c.col() << ")";
}
