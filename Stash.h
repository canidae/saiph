#ifndef STASH_H
#define STASH_H

#include <list>
#include "Globals.h"
#include "Item.h"

class Stash {
public:
	int last_look;
	unsigned char top_symbol;
	int top_color;
	std::list<Item> items;

	Stash(int last_look = 0, unsigned char top_symbol = ILLEGAL_ITEM, int top_color = NO_COLOR) : last_look(last_look), top_symbol(top_symbol), top_color(top_color), items() {
	}
};
#endif
