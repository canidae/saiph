#include "Stash.h"

using namespace std;

/* constructors/destructor */
Stash::Stash(const unsigned char& symbol, const int& color, const int& last_inspected) : _symbol(symbol), _color(color), _items(), _last_inspected(last_inspected) {
}

/* methods */
const unsigned char& Stash::symbol() const {
	return _symbol;
}

const unsigned char& Stash::symbol(const unsigned char& symbol) {
	_symbol = symbol;
	return this->symbol();
}

const int& Stash::color() const {
	return _color;
}

const int& Stash::color(const int& color) {
	_color = color;
	return this->color();
}

list<Item>& Stash::items() {
	return _items;
}

const int& Stash::lastInspected() const {
	return _last_inspected;
}

const int& Stash::lastInspected(const int& last_inspected) {
	_last_inspected = last_inspected;
	return this->lastInspected();
}