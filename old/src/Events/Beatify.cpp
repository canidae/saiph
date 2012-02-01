#include "Events/Beatify.h"

using namespace event;

/* constructors/destructor */
Beatify::Beatify(unsigned char key, int priority) : Event("Beatify"), _key(key), _priority(priority) {
}

Beatify::~Beatify() {
}

/* methods */
int Beatify::id() {
	return ID;
}

unsigned char Beatify::key() {
	return _key;
}

int Beatify::priority() {
	return _priority;
}
