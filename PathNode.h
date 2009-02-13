#ifndef PATHNODE_H
#define PATHNODE_H

#include <limits.h>
#include "Globals.h"
#include "Point.h"

class PathNode {
	public:
		Point next;
		unsigned char dir;
		int moves;
		unsigned int cost;

		PathNode(const Point &next = Point(), unsigned char dir = ILLEGAL_DIRECTION, int moves = -1, unsigned int cost = UINT_MAX);
};
#endif
