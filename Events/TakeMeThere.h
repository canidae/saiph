#ifndef EVENT_TAKE_ME_THERE_H
#define EVENT_TAKE_ME_THERE_H

#include "Event.h"
#include "../Coordinate.h"

namespace event {
	class TakeMeThere : public Event {
	public:
		static int id;
		Coordinate coordinate;
		int max_priority;

		TakeMeThere() : coordinate(), max_priority(0) {}
		virtual ~TakeMeThere() {}

		virtual int getID() {return id;}
		virtual std::string getName() {return "TakeMeThere";}
	};
}
#endif
