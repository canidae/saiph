#include "Events/GotDiggingTool.h"

using namespace event;

/* constructors/destructor */
GotDiggingTool::GotDiggingTool(unsigned char key) : Event("GotDiggingTool"), _key(key) {
}

GotDiggingTool::~GotDiggingTool() {
}

/* methods */
int GotDiggingTool::id() {
	return ID;
}

unsigned char GotDiggingTool::key() const {
	return _key;
}
