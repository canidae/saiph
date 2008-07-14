#include "Coordinate.h"

/* constructors */
Coordinate::Coordinate(const Point &point, int branch, int level) : Point(point) {
	this->branch = branch;
	this->level = level;
}
