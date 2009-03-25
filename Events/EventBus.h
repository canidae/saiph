#ifndef _EVENTBUS_H
#define	_EVENTBUS_H

#include "Event.h"
#include "../Analyzer.h"
#include <vector>

class EventBus {
public:
	//I'd call this "register", but that's a keyword!
	static void add(int event_type, Analyzer *analyzer);
	static void remove(int event_type, Analyzer *analyzer);
	static void broadcast(Event &evt);
private:
	static std::vector< std::vector<Analyzer *> > analyzers;
};

#endif	/* _EVENTBUS_H */

