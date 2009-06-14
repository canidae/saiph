#include "Event.h"
#include "ChangedInventoryItems.h"
#include "DropInventoryItems.h"
#include "ElberethQuery.h"
#include "ItemsOnGround.h"
#include "PickupItems.h"
#include "ReceivedItems.h"
#include "../EventBus.h"

using namespace event;

/* define static variables */
bool Event::initialized = false;
int ChangedInventoryItems::id = ILLEGAL_EVENT_ID;
int DropInventoryItems::id = ILLEGAL_EVENT_ID;
int ElberethQuery::id = ILLEGAL_EVENT_ID;
int ItemsOnGround::id = ILLEGAL_EVENT_ID;
int PickupItems::id = ILLEGAL_EVENT_ID;
int ReceivedItems::id = ILLEGAL_EVENT_ID;

/* methods */
void Event::init() {
	if (initialized)
		return;
	initialized = true;

	/* init events */
	ChangedInventoryItems::id = EventBus::createEventID();
	DropInventoryItems::id = EventBus::createEventID();
	ElberethQuery::id = EventBus::createEventID();
	ItemsOnGround::id = EventBus::createEventID();
	PickupItems::id = EventBus::createEventID();
	ReceivedItems::id = EventBus::createEventID();
}

void Event::destroy() {
}
