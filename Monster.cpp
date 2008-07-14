#include "Monster.h"

/* constructors */
Monster::Monster(const Point &point, unsigned char symbol, int color) : Point(point) {
	this->symbol = symbol;
	this->color = color;
}
