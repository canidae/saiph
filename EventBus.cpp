#include "Debug.h"
#include "EventBus.h"
#include "Analyzers/Analyzer.h"

#define EVENTBUS_DEBUG_NAME "EventBus] "

using namespace analyzer;
using namespace event;
using namespace std;

vector<vector<Analyzer *> > EventBus::events;

int EventBus::createEventID() {
	int event_id = EventBus::events.size();
	EventBus::events.resize(event_id + 1);
	return event_id;
}

void EventBus::registerEvent(int event_id, Analyzer *analyzer) {
	if (event_id < 0 || event_id >= (int) EventBus::events.size())
		return;
	Debug::notice() << EVENTBUS_DEBUG_NAME << "Registering " << analyzer->name << " for event " << event_id << endl;
	events[event_id].push_back(analyzer);
}

void EventBus::unregisterEvent(int event_id, Analyzer *analyzer) {
	if (event_id < 0 || event_id >= (int) EventBus::events.size())
		return;
	vector<Analyzer *> &subscribers = events[event_id];
	for (vector<Analyzer *>::iterator s = subscribers.begin(); s != subscribers.end(); ++s) {
		if (*s == analyzer) {
			subscribers.erase(s);
			Debug::notice() << EVENTBUS_DEBUG_NAME << "Unregistered " << analyzer->name << " for event " << event_id << endl;
			return;
		}
	}
	Debug::warning() << EVENTBUS_DEBUG_NAME << "Failed to unregister " << analyzer->name << " for event " << event_id << endl;
}

void EventBus::broadcast(Event *const event) {
	if (event->getID() < 0 || event->getID() >= (int) EventBus::events.size())
		return;
	vector<Analyzer *> &subscribers = events[event->getID()];
	for (vector<Analyzer *>::iterator s = subscribers.begin(); s != subscribers.end(); ++s)
		(*s)->onEvent(event);
}
