#ifndef STASH_H
/* defines */
#define STASH_H

/* forward declare */
class Stash;

/* includes */
#include <list>
#include "Item.h"

/* namespaces */
using namespace std;

/* Stash */
class Stash {
	public:
		/* variables */
		int frame_changed;
		list<Item> items;

		/* constructors */
		Stash(int frame_changed = -1);

		/* methods */
		void addItem(const Item &item);
		void removeItem(const Item &item);
};
#endif
