#include "Coordinate.h"

using namespace std;

/* constructors/destructors */
Coordinate::Coordinate(int level, int row, int col) : Point(row, col), _level(level) {
}

Coordinate::Coordinate(int level, const Point& point) : Point(point), _level(level) {
}

/* methods */
int Coordinate::level() const {
	return _level;
}

int Coordinate::level(int level) {
	_level = level;
	return this->level();
}

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

bool Coordinate::operator==(const Coordinate& c) const {
	return (_level == c._level && row() == c.row() && col() == c.col());
}

bool Coordinate::operator!=(const Coordinate& c) const {
	return (_level != c._level || row() != c.row() || col() != c.col());
}

ostream& operator<<(ostream& os, const Coordinate& c) {
	return os << "(" << c.level() << ", " << c.row() << ", " << c.col() << ")";
}
