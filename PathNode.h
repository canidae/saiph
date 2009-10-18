#ifndef PATHNODE_H
#define PATHNODE_H

#include <limits.h>
#include "Globals.h"
#include "Point.h"

class PathNode {
public:
	PathNode(const Point& next = Point(), unsigned char dir = ILLEGAL_DIRECTION, unsigned int moves = UNREACHABLE, unsigned int cost = UNREACHABLE);

	const Point& next() const;
	const Point& next(const Point& next);
	unsigned char dir() const;
	unsigned char dir(unsigned char dir);
	unsigned int moves() const;
	unsigned int moves(unsigned int moves);
	unsigned int cost() const;
	unsigned int cost(unsigned int cost);

private:
	Point _next;
	unsigned char _dir;
	unsigned int _moves;
	unsigned int _cost;
};
#endif
