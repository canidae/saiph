#ifndef STASH_H
#define STASH_H

#include <list>
#include "Globals.h"
#include "Item.h"

class Stash {
public:
	Stash(const unsigned char& symbol = ILLEGAL_ITEM, const int& color = NO_COLOR, const int& last_inspected = 0);

	const unsigned char& symbol() const;
	const unsigned char& symbol(const unsigned char& symbol);
	const int& color() const;
	const int& color(const int& color);
	std::list<Item>& items();
	const int& lastInspected() const;
	const int& lastInspected(const int& last_inspected);

private:
	unsigned char _symbol;
	int _color;
	std::list<Item> _items;
	int _last_inspected;
};
#endif
