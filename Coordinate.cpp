#include "Coordinate.h"

/* constructors */
Coordinate::Coordinate(int branch, int level, int row, int col) : Point(row, col) {
	this->branch = branch;
	this->level = level;
}

Coordinate::Coordinate(const Point &point, int branch, int level) : Point(point) {
	this->branch = branch;
	this->level = level;
}
