#ifndef _EVENTBUS_H
#define	_EVENTBUS_H

#include <vector>
#include "Events/Event.h"

class Analyzer;

class EventBus {
public:
	static int createEventType();
	static void registerEvent(int event_type, Analyzer *analyzer);
	static void unregisterEvent(int event_type, Analyzer *analyzer);
	static void broadcast(event::Event *const event);

private:
	static std::vector<std::vector<Analyzer *> > events;
};
#endif
