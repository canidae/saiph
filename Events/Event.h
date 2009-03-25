#ifndef _EVENT_H
#define	_EVENT_H

/* the registry of event types; must be consecutive integers */
#define EVENT_INVENTORY_CHANGED 0 /* InventoryChangedEvent */
#define EVENT_PICKUP_QUERY 1 /* PickupQueryEvent */
#define NUM_EVENT_TYPES 2 /* needed in EventBus to make the right size vector */

class Event {
public:
	Event(const int type) : type(type) {}
	const int type;
};

#endif	/* _EVENT_H */

