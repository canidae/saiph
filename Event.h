#ifndef _EVENT_H
#define	_EVENT_H

/* It may seem dumb to devote an entire header file to Event, rather than
 * tucking it in EventBus.h, but otherwise we get a circular #include
 * dependency with Analyzer.h #including EventBus.h and EventBus.h #including
 * Analyzer.h. */

#define EVENT_INVENTORY_UPDATED 0 /* contains information about which items changed */
#define EVENT_REQUEST_DIRTY_INVENTORY 1 /* we want updated inventory */
#define EVENT_REQUEST_PRAY 2 /* we want prayer; has some urgency level */
#define NUM_EVENT_TYPES 3 /* needed in EventBus to make the right size vector */

class Event {
public:
	Event(const int type) : type(type) {}
	const int type;
};

#endif	/* _EVENT_H */

