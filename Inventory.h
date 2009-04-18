#ifndef _INVENTORY_H
#define _INVENTORY_H

#include <map>
#include <string>
#include "Debug.h"
#include "Item.h"
#include "World.h"

#define INVENTORY_DEBUG_NAME "Inventory] "

#define MESSAGE_NOT_CARRYING_ANYTHING "  Not carrying anything.  "
#define MESSAGE_NOT_CARRYING_ANYTHING_EXCEPT_GOLD "  Not carrying anything except gold.  "

class Inventory {
public:
	static bool updated;
	static std::map<unsigned char, Item> items;

	static void addItem(unsigned char key, const Item &item);
	static void removeItem(unsigned char key, const Item &item);
	static void parseMessages(const std::string &messages);

private:
	static std::vector<unsigned char> changed_items;
};
#endif
