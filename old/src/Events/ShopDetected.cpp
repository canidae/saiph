#include "Events/ShopDetected.h"

using namespace event;

/* constructors/destructor */
ShopDetected::ShopDetected(const Point& ul, const Point& lr) : Event("ShopDetected"), _ul(ul), _lr(lr) {
}

ShopDetected::~ShopDetected() {
}

/* methods */
int ShopDetected::id() {
	return ID;
}

const Point& ShopDetected::ul() const {
	return _ul;
}

const Point& ShopDetected::lr() const {
	return _lr;
}
