#ifndef EVENTBUS_H
#define EVENTBUS_H

#include <vector>
#include "Events/Event.h"

namespace analyzer {
	class Analyzer;
}

class EventBus {
public:
	static void registerEvent(const int& event_id, analyzer::Analyzer * const analyzer);
	static void unregisterEvent(const int& event_id, analyzer::Analyzer * const analyzer);
	static void broadcast(event::Event * const event);

private:
	static std::vector<std::vector<analyzer::Analyzer*> > _events;
};
#endif
