#ifndef _EVENT_H
#define	_EVENT_H

/* the registry of event types; must be consecutive integers */
#define EVENT_INVENTORY_CHANGED 0 /* InventoryChangedEvent */
#define EVENT_REQUEST_DIRTY_INVENTORY 1 /* we want updated inventory */
#define EVENT_REQUEST_PRAY 2 /* we want prayer; has some urgency level */
#define NUM_EVENT_TYPES 3 /* needed in EventBus to make the right size vector */

class Event {
public:
	Event(const int type) : type(type) {}
	const int type;
};

#endif	/* _EVENT_H */

