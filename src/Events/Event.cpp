#include "Events/Event.h"

#include "EventBus.h"
#include "Events/Beatify.h"
#include "Events/ChangedInventoryItems.h"
#include "Events/EatItem.h"
#include "Events/ElberethQuery.h"
#include "Events/ItemsOnGround.h"
#include "Events/ReceivedItems.h"
#include "Events/StashChanged.h"
#include "Events/TakeMeThere.h"
#include "Events/WantItems.h"

using namespace event;

/* define static variables */
const int ChangedInventoryItems::ID = 0;
const int EatItem::ID = 1;
const int ElberethQuery::ID = 2;
const int ItemsOnGround::ID = 3;
const int ReceivedItems::ID = 4;
const int StashChanged::ID = 5;
const int TakeMeThere::ID = 6;
const int WantItems::ID = 7;
const int Beatify::ID = 8;

/* constructors/destructor */
Event::Event(const std::string& name) : _name(name) {
}

Event::~Event() {
}

/* methods */
const std::string& Event::name() {
	return _name;
}
