#include "Stash.h"

/* constructors */
Stash::Stash(int frame_changed, unsigned char top_symbol) : frame_changed(frame_changed), top_symbol(top_symbol) {
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
		++i;
	}
}
