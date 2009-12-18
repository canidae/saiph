#include "Debug.h"
#include "EventBus.h"
#include "Analyzers/Analyzer.h"

using namespace analyzer;
using namespace event;
using namespace std;

vector<vector<Analyzer*> > EventBus::_events;

void EventBus::registerEvent(int event_id, Analyzer * const analyzer) {
	if (event_id < 0)
		return;
	if (event_id >= (int) _events.size())
		_events.resize(event_id + 1);
	Debug::event() << "Registering " << analyzer->name() << " for event " << event_id << endl;
	_events[event_id].push_back(analyzer);
}

void EventBus::unregisterEvent(int event_id, Analyzer * const analyzer) {
	if (event_id < 0 || event_id >= (int) _events.size())
		return;
	vector<Analyzer*>& subscribers = _events[event_id];
	for (vector<Analyzer*>::iterator s = subscribers.begin(); s != subscribers.end(); ++s) {
		if (*s == analyzer) {
			subscribers.erase(s);
			Debug::event() << "Unregistered " << analyzer->name() << " for event " << event_id << endl;
			return;
		}
	}
	Debug::warning() << "Failed to unregister " << analyzer->name() << " for event " << event_id << endl;
}

void EventBus::broadcast(Event * const event) {
	if (event->id() < 0 || event->id() >= (int) _events.size())
		return;
	vector<Analyzer*>& subscribers = _events[event->id()];
	Debug::event() << "Broadcasting " << event->name() << " to " << subscribers.size() << " subscribers" << endl;
	for (vector<Analyzer*>::iterator s = subscribers.begin(); s != subscribers.end(); ++s)
		(*s)->onEvent(event);
}
