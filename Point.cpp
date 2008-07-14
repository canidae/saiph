#include "Point.h"

/* constructors */
Point::Point(int row, int col) {
	this->row = row;
	this->col = col;
}

Point::Point() {
	this->row = 0;
	this->col = 0;
}
