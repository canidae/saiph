#ifndef _CHANGED_INVENTORY_ITEMS_H
#define _CHANGED_INVENTORY_ITEMS_H

#include <vector>
#include "Event.h"

namespace event {
	class ChangedInventoryItems {
	public:
		static int id;
		std::vector<unsigned char> item_keys;

		ChangedInventoryItems(const std::vector<unsigned char> &item_keys) : item_keys(item_keys) {}
		virtual ~ChangedInventoryItems() {}

		virtual int getID() {return id;}
		virtual std::string getName() {return "ChangedInventoryItems";}
	};
}
#endif
