#include "Events/ItemsOnGround.h"

using namespace event;

/* constructors/destructor */
ItemsOnGround::ItemsOnGround(const std::list<Item>& items) : Event("ItemsOnGround"), _items(items) {
}

ItemsOnGround::~ItemsOnGround() {
}

/* methods */
int ItemsOnGround::id() {
	return ID;
}

const std::list<Item>& ItemsOnGround::items() const {
	return _items;
}
