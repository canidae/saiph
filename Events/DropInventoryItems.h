#ifndef EVENT_DROP_INVENTORY_ITEMS_H
#define EVENT_DROP_INVENTORY_ITEMS_H

#include <set>
#include "Event.h"

namespace event {
	class DropInventoryItems : public Event {
	public:
		static int id;
		std::set<unsigned char> keys;

		DropInventoryItems() : keys() {}
		virtual ~DropInventoryItems() {}

		virtual int getID() {return id;}
		virtual std::string getName() {return "DropInventoryItems";}
	};
}
#endif
