#include "Event.h"
#include "ChangedInventoryItems.h"
#include "EatItem.h"
#include "ElberethQuery.h"
#include "ItemsOnGround.h"
#include "ReceivedItems.h"
#include "StashChanged.h"
#include "TakeMeThere.h"
#include "WantItems.h"
#include "../EventBus.h"

using namespace event;

/* define static variables */
bool Event::initialized = false;
int ChangedInventoryItems::id = ILLEGAL_EVENT_ID;
int EatItem::id = ILLEGAL_EVENT_ID;
int ElberethQuery::id = ILLEGAL_EVENT_ID;
int ItemsOnGround::id = ILLEGAL_EVENT_ID;
int ReceivedItems::id = ILLEGAL_EVENT_ID;
int StashChanged::id = ILLEGAL_EVENT_ID;
int TakeMeThere::id = ILLEGAL_EVENT_ID;
int WantItems::id = ILLEGAL_EVENT_ID;

/* methods */
void Event::init() {
	if (initialized)
		return;
	initialized = true;

	/* init events */
	ChangedInventoryItems::id = EventBus::createEventID();
	EatItem::id = EventBus::createEventID();
	ElberethQuery::id = EventBus::createEventID();
	ItemsOnGround::id = EventBus::createEventID();
	ReceivedItems::id = EventBus::createEventID();
	StashChanged::id = EventBus::createEventID();
	TakeMeThere::id = EventBus::createEventID();
	WantItems::id = EventBus::createEventID();
}

void Event::destroy() {
}
