#include "Events/ReceivedItems.h"

using namespace event;

/* constructors/destructor */
ReceivedItems::ReceivedItems() : Event("ReceivedItems"), _items() {
}

ReceivedItems::~ReceivedItems() {
}

/* methods */
int ReceivedItems::id() {
	return ID;
}

std::map<unsigned char, Item>& ReceivedItems::items() {
	return _items;
}

std::map<unsigned char, Item>& ReceivedItems::items(const std::map<unsigned char, Item>& items) {
	_items = items;
	return this->items();
}

void ReceivedItems::clear() {
	_items.clear();
}

void ReceivedItems::addItem(unsigned char key, const Item& item) {
	_items[key] = item;
}
