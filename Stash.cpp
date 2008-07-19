#include "Stash.h"

/* constructors */
Stash::Stash(const Coordinate &coordinate, unsigned char top_item) : Coordinate(coordinate), top_item(top_item) {
}

Stash::Stash(unsigned char top_item) : top_item(top_item) {
}

/* methods */
void Stash::addItem(const Item &item) {
	items.push_back(item);
}

void Stash::removeItem(const Item &item) {
	for (list<Item>::iterator i = items.begin(); i != items.end(); ) {
		if (item.name == i->name) {
			if (item.count < i->count)
				i->count -= item.count;
			else
				i = items.erase(i);
			return;
		}
	}
}
