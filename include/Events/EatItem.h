#ifndef EVENT_EAT_ITEM_H
#define EVENT_EAT_ITEM_H

#include "Events/Event.h"

namespace event {

	class EatItem : public Event {
	public:
		static const int ID;

		EatItem() : Event("EatItem"), _key(0), _priority(0) {
		}

		virtual ~EatItem() {
		}

		virtual int id() {
			return ID;
		}

		virtual unsigned char key() {
			return _key;
		}

		virtual int priority() {
			return _priority;
		}

	private:
		unsigned char _key;
		int _priority;
	};
}
#endif
