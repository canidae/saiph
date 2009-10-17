#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <vector>
#include "Events/Event.h"

namespace analyzer {
	class Analyzer;
}

class EventBus {
public:
	static int createEventID();
	static void registerEvent(int event_id, analyzer::Analyzer *analyzer);
	static void unregisterEvent(int event_id, analyzer::Analyzer *analyzer);
	static void broadcast(event::Event * const event);

private:
	static std::vector<std::vector<analyzer::Analyzer *> > events;
};
#endif
