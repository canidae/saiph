#include "Events/EatItem.h"

using namespace event;

/* constructors/destructor */
EatItem::EatItem() : Event("EatItem"), _key(0), _priority(0) {
}

EatItem::~EatItem() {
}

/* methods */
int EatItem::id() {
	return ID;
}

unsigned char EatItem::key() {
	return _key;
}

int EatItem::priority() {
	return _priority;
}
