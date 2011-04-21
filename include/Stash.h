#ifndef STASH_H
#define STASH_H

#include <list>
#include "Globals.h"
#include "Item.h"

class Stash {
public:
	Stash(unsigned char symbol = ILLEGAL_ITEM, int color = NO_COLOR, int last_inspected = 0);

	unsigned char symbol() const { return _symbol; }
	unsigned char symbol(unsigned char symbol);
	int color() const { return _color; }
	int color(int color);
	std::list<Item>& items() { return _items; }
	int lastInspected() const { return _last_inspected; }
	int lastInspected(int last_inspected);

private:
	unsigned char _symbol;
	int _color;
	std::list<Item> _items;
	int _last_inspected;
};
#endif
