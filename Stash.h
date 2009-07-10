#ifndef STASH_H
#define STASH_H

#include <list>
#include "Globals.h"
#include "Item.h"

class Stash {
public:
	unsigned char top_symbol;
	int top_color;
	std::list<Item> items;

	Stash(unsigned char top_symbol = ILLEGAL_ITEM, int top_color = NO_COLOR);
};
#endif
