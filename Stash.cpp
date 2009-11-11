#include "Stash.h"

using namespace std;

/* constructors/destructor */
Stash::Stash(unsigned char symbol, int color, int last_inspected) : _symbol(symbol), _color(color), _items(), _last_inspected(last_inspected) {
}

/* methods */
unsigned char Stash::symbol() const {
	return _symbol;
}

unsigned char Stash::symbol(unsigned char symbol) {
	_symbol = symbol;
	return this->symbol();
}

int Stash::color() const {
	return _color;
}

int Stash::color(int color) {
	_color = color;
	return this->color();
}

list<Item>& Stash::items() {
	return _items;
}

int Stash::lastInspected() const {
	return _last_inspected;
}

int Stash::lastInspected(int last_inspected) {
	_last_inspected = last_inspected;
	return this->lastInspected();
}
