#include "Events/StashChanged.h"

using namespace event;

/* constructors/destructor */
StashChanged::StashChanged(const Coordinate& position) : Event("StashChanged"), _position(position) {
}

StashChanged::~StashChanged() {
}

/* methods */
int StashChanged::id() {
	return ID;
}

const Coordinate& StashChanged::position() const {
	return _position;
}
