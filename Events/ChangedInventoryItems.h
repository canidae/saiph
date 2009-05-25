#ifndef _CHANGED_INVENTORY_ITEMS_H
#define _CHANGED_INVENTORY_ITEMS_H

#include <set>
#include "Event.h"

namespace event {
	class ChangedInventoryItems {
	public:
		static int id;
		std::set<unsigned char> keys;

		ChangedInventoryItems(const std::set<unsigned char> &keys = std::set<unsigned char>()) : keys(keys) {}
		virtual ~ChangedInventoryItems() {}

		virtual int getID() {return id;}
		virtual std::string getName() {return "ChangedInventoryItems";}
	};
}
#endif
