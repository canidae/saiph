#include "Events/ChangedInventoryItems.h"

using namespace event;

/* constructors/destructor */
ChangedInventoryItems::ChangedInventoryItems() : Event("ChangedInventoryItems"), _keys() {
}

ChangedInventoryItems::~ChangedInventoryItems() {
}

/* methods */
int ChangedInventoryItems::id() {
	return ID;
}

std::set<unsigned char>& ChangedInventoryItems::keys() {
	return _keys;
}

std::set<unsigned char>& ChangedInventoryItems::keys(const std::set<unsigned char>& keys) {
	_keys = keys;
	return this->keys();
}

void ChangedInventoryItems::clear() {
	_keys.clear();
}

void ChangedInventoryItems::add(unsigned char key) {
	_keys.insert(key);
}
