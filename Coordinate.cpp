#include "Coordinate.h"

/* constructors */
Coordinate::Coordinate(int branch, int level, const Point &point) : Point(point) {
	this->branch = branch;
	this->level = level;
}
