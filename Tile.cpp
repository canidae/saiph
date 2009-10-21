#include "Tile.h"
#include "World.h"

/* constructors/destructor */
Tile::Tile() : _symbol(SOLID_ROCK), _monster(ILLEGAL_MONSTER), _direction(ILLEGAL_DIRECTION), _search(0), _distance(UNREACHABLE), _cost(UNREACHABLE), _updated(0), _next(Point()) {
}

Tile::~Tile() {
}

/* methods */
unsigned int Tile::cost() const {
	if (_updated != World::internalTurn())
		return UNREACHABLE;
	return _cost;
}

unsigned char Tile::direction() const {
	if (_updated != World::internalTurn())
		return ILLEGAL_DIRECTION;
	return _direction;
}

unsigned int Tile::distance() const {
	if (_updated != World::internalTurn())
		return UNREACHABLE;
	return _distance;
}

unsigned char Tile::monster() const {
	return _monster;
}

unsigned int Tile::search() const {
	return _search;
}

unsigned int Tile::searchInc() {
	if (_search < TILE_FULLY_SEARCHED)
		return ++_search;
	return _search;
}

unsigned char Tile::symbol() const {
	return _symbol;
}

unsigned char Tile::monster(unsigned char monster) {
	_monster = monster;
	return this->monster();
}

unsigned int Tile::search(unsigned int search) {
	_search = search;
	return this->search();
}

unsigned char Tile::symbol(unsigned char symbol) {
	_symbol = symbol;
	return this->symbol();
}

void Tile::updatePath(const Point& next, unsigned char direction, unsigned int distance, unsigned int cost) {
	_next = next;
	_direction = direction;
	_distance = distance;
	_cost = cost;
	_updated = World::internalTurn();
}