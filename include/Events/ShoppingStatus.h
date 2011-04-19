#ifndef EVENT_SHOPPING_STATUS_H
#define EVENT_SHOPPING_STATUS_H

#include "Events/Event.h"

namespace event {

	class ShoppingStatus : public Event {
	public:
		static const int ID;

		ShoppingStatus(bool nstate) : Event("ShoppingStatus"), _new_state(nstate) {
		}

		virtual ~ShoppingStatus() {
		}

		virtual int id() {
			return ID;
		}

		bool newState() const {
			return _new_state;
		}

	private:
		bool _new_state;
	};
}
#endif
