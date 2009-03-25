#include "EventBus.h"
#include "../Debug.h"

#define EVENTBUS_DEBUG_NAME "EventBus] "

using namespace std;

vector< vector<Analyzer *> > EventBus::analyzers(NUM_EVENT_TYPES);

void EventBus::add(int event_type, Analyzer* analyzer) {
	Debug::notice() << EVENTBUS_DEBUG_NAME << "Registering " << analyzer->name << " for event " << event_type << endl;
	analyzers[event_type].push_back(analyzer);
}

void EventBus::remove(int event_type, Analyzer* analyzer) {
	vector<Analyzer *> &subscribers = analyzers[event_type];
	for (vector<Analyzer *>::size_type i = 0; i < subscribers.size(); ++i)
		if (subscribers[i] == analyzer) {
			subscribers.erase(subscribers.begin() + i);
			Debug::notice() << EVENTBUS_DEBUG_NAME << "Deregistered " << analyzer->name << " for event " << event_type << endl;
			return;
		}
	Debug::warning() << EVENTBUS_DEBUG_NAME << "Failed to deregister " << analyzer->name << " for event " << event_type << endl;
}

void EventBus::broadcast(Event& evt) {
	const vector<Analyzer *> &subscribers = analyzers[evt.type];
	for (vector<Analyzer *>::size_type i = 0; i < subscribers.size(); ++i)
		subscribers[i]->handle(evt);
}
