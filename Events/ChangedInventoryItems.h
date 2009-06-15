#ifndef EVENT_CHANGED_INVENTORY_ITEMS_H
#define EVENT_CHANGED_INVENTORY_ITEMS_H

#include <set>
#include "Event.h"

namespace event {
	class ChangedInventoryItems : public Event {
	public:
		static int id;
		std::set<unsigned char> keys;

		ChangedInventoryItems() : keys() {}
		virtual ~ChangedInventoryItems() {}

		virtual int getID() {return id;}
		virtual std::string getName() {return "ChangedInventoryItems";}
	};
}
#endif
