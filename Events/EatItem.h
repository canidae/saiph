#ifndef EVENT_EAT_ITEM_H
#define EVENT_EAT_ITEM_H

#include "Event.h"

namespace event {

	class EatItem : public Event {
	public:
		static int id;
		unsigned char key;
		int priority;

		EatItem() : key(0), priority(0) {
		}

		virtual ~EatItem() {
		}

		virtual int getID() {
			return id;
		}

		virtual std::string getName() {
			return "EatItem";
		}
	};
}
#endif
