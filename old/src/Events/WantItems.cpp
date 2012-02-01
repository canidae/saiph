#include "Events/WantItems.h"

using namespace event;

/* constructors/destructor */
WantItems::WantItems(bool dropping, bool safe_stash) : Event("WantItems"), _dropping(dropping), _safe_stash(safe_stash), _items() {
}

WantItems::~WantItems() {
}

/* methods */
int WantItems::id() {
	return ID;
}

std::map<unsigned char, Item>& WantItems::items() {
	return _items;
}

std::map<unsigned char, Item>& WantItems::items(const std::map<unsigned char, Item>& items) {
	_items = items;
	return this->items();
}

void WantItems::clear() {
	_items.clear();
}

void WantItems::addItem(unsigned char key, const Item& item) {
	_items[key] = item;
}

bool WantItems::dropping() {
	return _dropping;
}

bool WantItems::safeStash() {
	return _safe_stash;
}
