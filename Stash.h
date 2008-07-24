#ifndef STASH_H
/* defines */
#define STASH_H

/* forward declare */
class Stash;

/* includes */
#include <list>
#include "Globals.h"
#include "Item.h"

/* namespaces */
using namespace std;

/* Stash */
class Stash {
	public:
		/* variables */
		int frame_changed;
		unsigned char top_symbol;
		list<Item> items;

		/* constructors */
		Stash(int frame_changed = -1, unsigned char top_symbol = ILLEGAL_ITEM);

		/* methods */
		void addItem(const Item &item);
		void removeItem(const Item &item);
};
#endif
