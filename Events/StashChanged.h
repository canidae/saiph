#ifndef EVENT_STASH_CHANGED_H
#define EVENT_STASH_CHANGED_H

#include <list>
#include "Event.h"
#include "../Coordinate.h"

namespace event {

	class StashChanged : public Event {
	public:
		static const int ID;

		StashChanged(const Coordinate& position) : Event("StashChanged"), _position(position) {
		}

		virtual ~StashChanged() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Coordinate& position() const {
			return _position;
		}

	private:
		const Coordinate _position;
	};
}
#endif
