#ifndef EVENT_STASH_CHANGED_H
#define EVENT_STASH_CHANGED_H

#include "Coordinate.h"
#include "Events/Event.h"

namespace event {
	class StashChanged : public Event {
	public:
		static const int ID;

		StashChanged(const Coordinate& position);
		virtual ~StashChanged();

		virtual int id();
		virtual const Coordinate& position() const;

	private:
		const Coordinate _position;
	};
}
#endif
