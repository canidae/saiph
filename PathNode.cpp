#include "PathNode.h"

PathNode::PathNode(const Point& next, unsigned char dir, unsigned int moves, unsigned int cost) : next(next), dir(dir), moves(moves), cost(cost) {
}
