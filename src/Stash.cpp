#include "Stash.h"

using namespace std;

/* constructors/destructor */
Stash::Stash(unsigned char symbol, int color, int last_inspected) : _symbol(symbol), _color(color), _items(), _last_inspected(last_inspected) {
}

/* methods */
unsigned char Stash::symbol(unsigned char symbol) {
	_symbol = symbol;
	return this->symbol();
}

int Stash::color(int color) {
	_color = color;
	return this->color();
}

int Stash::lastInspected(int last_inspected) {
	_last_inspected = last_inspected;
	return this->lastInspected();
}
