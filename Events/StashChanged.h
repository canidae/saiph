#ifndef EVENT_STASH_CHANGED_H
#define EVENT_STASH_CHANGED_H

#include <list>
#include "Event.h"
#include "../Coordinate.h"

namespace event {
	class StashChanged : public Event {
	public:
		static int id;
		Coordinate stash;

		StashChanged() : stash() {}
		virtual ~StashChanged() {}

		virtual int getID() {return id;}
		virtual std::string getName() {return "StashChanged";}
	};
}
#endif
