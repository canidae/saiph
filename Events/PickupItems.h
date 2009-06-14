#ifndef _PICKUP_ITEMS_H
#define _PICKUP_ITEMS_H

#include <map>
#include "Event.h"
#include "../Item.h"

namespace event {
	class PickupItems : public Event {
	public:
		static int id;
		std::map<unsigned char, Item> items;

		PickupItems() : items() {}
		virtual ~PickupItems() {}

		virtual int getID() {return id;}
		virtual std::string getName() {return "PickupItems";}
	};
}
#endif
