#include "Tile.h"
#include "World.h"

/* static */
unsigned int Tile::_unreachable = UNREACHABLE;
unsigned char Tile::_illegal_direction = ILLEGAL_DIRECTION;

/* constructors/destructor */
Tile::Tile() : _symbol(SOLID_ROCK), _monster(ILLEGAL_MONSTER), _direction(ILLEGAL_DIRECTION), _search(0), _distance(UNREACHABLE), _cost(UNREACHABLE), _updated(0), _next(Point()) {
}

Tile::~Tile() {
}

/* methods */
const unsigned int& Tile::cost() const {
	if (_updated != World::internalTurn())
		return _unreachable;
	return _cost;
}

const unsigned char& Tile::direction() const {
	if (_updated != World::internalTurn())
		return _illegal_direction;
	return _direction;
}

const unsigned char& Tile::direction(const unsigned char& direction) {
	_direction = direction;
	return this->direction();
}

const unsigned int& Tile::distance() const {
	if (_updated != World::internalTurn())
		return _unreachable;
	return _distance;
}

const unsigned char& Tile::monster() const {
	return _monster;
}

const unsigned char& Tile::monster(const unsigned char& monster) {
	_monster = monster;
	return this->monster();
}

const unsigned int& Tile::search() const {
	return _search;
}

const unsigned int& Tile::search(const unsigned int& search) {
	_search = search;
	return this->search();
}

const unsigned int& Tile::searchInc() {
	if (_search < TILE_FULLY_SEARCHED)
		return ++_search;
	return _search;
}

const unsigned char& Tile::symbol() const {
	return _symbol;
}

const unsigned char& Tile::symbol(const unsigned char& symbol) {
	_symbol = symbol;
	return this->symbol();
}

void Tile::updatePath(const Point& next, const unsigned char& direction, const unsigned int& distance, const unsigned int& cost) {
	_next = next;
	_direction = direction;
	_distance = distance;
	_cost = cost;
	_updated = World::internalTurn();
}