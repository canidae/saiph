#ifndef _CALLINVENTORYITEMREQUESTEVENT_H
#define _CALLINVENTORYITEMREQUESTEVENT_H

#include "Event.h"
#include "EventBus.h"

class CallInventoryItemRequestEvent : Event {
public:
	static const int type;

	CallInventoryItemRequestEvent() {}
	~CallInventoryItemRequestEvent() {}

	int getType() {return type;}
};

const int CallInventoryItemRequestEvent::type = EventBus::createEventType();
#endif
