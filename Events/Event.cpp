#include "Event.h"
#include "ChangedInventoryItems.h"
#include "../EventBus.h"

using namespace event;

/* define static variables */
bool Event::initialized = false;
int ChangedInventoryItems::id = ILLEGAL_EVENT_ID;

/* methods */
void Event::init() {
	if (initialized)
		return;
	initialized = true;

	/* init events */
	ChangedInventoryItems::id = EventBus::createEventID();
}

void Event::destroy() {
}
