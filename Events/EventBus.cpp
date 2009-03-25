#include "EventBus.h"
#include "../Debug.h"

#define EVENTBUS_DEBUG_NAME "EventBus] "

using namespace std;

vector<vector<Analyzer *> > EventBus::events;

int EventBus::createEventType() {
	int event_type = EventBus::events.size();
	EventBus::events.resize(event_type + 1);
	return event_type;
}

void EventBus::registerEvent(int event_type, Analyzer *analyzer) {
	if (event_type < 0 || event_type >= (int) EventBus::events.size())
		return;
	Debug::notice() << EVENTBUS_DEBUG_NAME << "Registering " << analyzer->name << " for event " << event_type << endl;
	events[event_type].push_back(analyzer);
}

void EventBus::unregisterEvent(int event_type, Analyzer *analyzer) {
	if (event_type < 0 || event_type >= (int) EventBus::events.size())
		return;
	vector<Analyzer *> &subscribers = events[event_type];
	for (vector<Analyzer *>::iterator s = subscribers.begin(); s != subscribers.end(); ++s) {
		if (*s == analyzer) {
			subscribers.erase(s);
			Debug::notice() << EVENTBUS_DEBUG_NAME << "Unregistered " << analyzer->name << " for event " << event_type << endl;
			return;
		}
	}
	Debug::warning() << EVENTBUS_DEBUG_NAME << "Failed to unregister " << analyzer->name << " for event " << event_type << endl;
}

void EventBus::broadcast(Event *event) {
	if (event->getType() < 0 || event->getType() >= (int) EventBus::events.size())
		return;
	vector<Analyzer *> &subscribers = events[event->getType()];
	for (vector<Analyzer *>::iterator s = subscribers.begin(); s != subscribers.end(); ++s)
		(*s)->handle(event);
}
