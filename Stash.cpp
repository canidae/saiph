#include "Stash.h"

/* constructors */
Stash::Stash(unsigned char top_symbol) : top_symbol(top_symbol) {
}

/* methods */
void Stash::addItem(const Item &item) {
	items.push_back(item);
}

void Stash::removeItem(const Item &item) {
	for (list<Item>::iterator i = items.begin(); i != items.end(); ) {
		if (item.name.find(i->name) == 0) {
			/* must use find because in a shop we see a food ration,
			 * but we pick up "food ration (unpaid ...)" */
			if (item.count < i->count)
				i->count -= item.count;
			else
				i = items.erase(i);
			return;
		}
		++i;
	}
}
