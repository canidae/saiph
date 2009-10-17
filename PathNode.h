#ifndef PATHNODE_H
#define PATHNODE_H

#include <limits.h>
#include "Globals.h"
#include "Point.h"

class PathNode {
public:
	Point next;
	unsigned char dir;
	unsigned int moves;
	unsigned int cost;

	PathNode(const Point &next = Point(), unsigned char dir = ILLEGAL_DIRECTION, unsigned int moves = UNREACHABLE, unsigned int cost = UNREACHABLE);
};
#endif
