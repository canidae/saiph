#include "ReceivedItems.h"
#include "../EventBus.h"

using namespace event;

/* define static variables */
const int ReceivedItems::id = EventBus::createEventID();

/* constructors/destructor */
ReceivedItems::ReceivedItems() {
}

ReceivedItems::~ReceivedItems() {
}

/* methods */
int ReceivedItems::getID() {
	return id;
}
