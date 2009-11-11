#ifndef EVENT_STASH_CHANGED_H
#define EVENT_STASH_CHANGED_H

#include <list>
#include "Event.h"
#include "../Coordinate.h"

namespace event {

	class StashChanged : public Event {
	public:
		static const int ID;

		StashChanged() : Event("StashChanged"), _stash() {
		}

		virtual ~StashChanged() {
		}

		virtual int id() {
			return ID;
		}

		virtual const Coordinate& stash() {
			return _stash;
		}

		virtual const Coordinate& stash(const Coordinate& stash) {
			_stash = stash;
			return this->stash();
		}

	private:
		Coordinate _stash;
	};
}
#endif
