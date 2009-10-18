#include "PathNode.h"

/* constructors/destructor */
PathNode::PathNode(const Point& next, unsigned char dir, unsigned int moves, unsigned int cost) : _next(next), _dir(dir), _moves(moves), _cost(cost) {
}

/* methods */
const Point& PathNode::next() const {
	return _next;
}

const Point& PathNode::next(const Point& next) {
	_next = next;
	return this->next();
}

unsigned char PathNode::dir() const {
	return _dir;
}

unsigned char PathNode::dir(unsigned char dir) {
	_dir = dir;
	return this->dir();
}

unsigned int PathNode::moves() const {
	return _moves;
}

unsigned int PathNode::moves(unsigned int moves) {
	_moves = moves;
	return this->moves();
}

unsigned int PathNode::cost() const {
	return _cost;
}

unsigned int PathNode::cost(unsigned int cost) {
	_cost = cost;
	return this->cost();
}