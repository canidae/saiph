#ifndef _INVENTORY_H
#define _INVENTORY_H

#include <map>
#include <string>
#include "Debug.h"
#include "Globals.h"
#include "Item.h"
#include "World.h"

#define INVENTORY_DEBUG_NAME "Inventory] "

#define MESSAGE_NOT_CARRYING_ANYTHING "  Not carrying anything.  "
#define MESSAGE_NOT_CARRYING_ANYTHING_EXCEPT_GOLD "  Not carrying anything except gold.  "

class Inventory {
public:
	static bool updated;
	static std::map<unsigned char, Item> items;
	static unsigned char slots[SLOTS];

	static void analyze();
	static void parseMessages(const std::string &messages);
	static void addItem(unsigned char key, const Item &item);
	static void removeItem(unsigned char key, const Item &item);

private:
	static std::vector<unsigned char> changed_items;

	static void setSlot(unsigned char key, const Item &item);
};
#endif
