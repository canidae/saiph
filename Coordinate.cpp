#include "Coordinate.h"

/* constructors */
Coordinate::Coordinate(int branch, int level, int row, int col) : Point(row, col) {
	this->branch = branch;
	this->level = level;
}

Coordinate::Coordinate(int branch, int level, const Point &point) : Point(point) {
	this->branch = branch;
	this->level = level;
}

Coordinate::Coordinate(const Point &point) : Point(point) {
	this->branch = 0;
	this->level = 0;
}
