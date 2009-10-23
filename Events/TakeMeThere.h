#ifndef EVENT_TAKE_ME_THERE_H
#define EVENT_TAKE_ME_THERE_H

#include "Event.h"
#include "../Coordinate.h"

namespace event {

	class TakeMeThere : public Event {
	public:
		static const int ID;

		TakeMeThere() : Event("TakeMeThere"), _coordinate(), _max_priority(0) {
		}

		virtual ~TakeMeThere() {
		}

		virtual const int& id() {
			return ID;
		}

		virtual const Coordinate& coordinate() {
			return _coordinate;
		}

		virtual const int& max_priority() {
			return _max_priority;
		}

	private:
		Coordinate _coordinate;
		int _max_priority;
	};
}
#endif
