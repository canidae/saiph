#ifndef STASH_H
#define STASH_H

#include <list>
#include "Globals.h"
#include "Item.h"

class Stash {
public:
	Stash(unsigned char symbol = ILLEGAL_ITEM, int color = NO_COLOR, int last_inspected = 0);

	unsigned char symbol() const;
	unsigned char symbol(unsigned char symbol);
	int color() const;
	int color(int color);
	std::list<Item>& items();
	int lastInspected() const;
	int lastInspected(int last_inspected);

private:
	unsigned char _symbol;
	int _color;
	std::list<Item> _items;
	int _last_inspected;
};
#endif
