#ifndef INVENTORY_H
#define INVENTORY_H

#include <map>
#include <set>
#include <string>
#include "Debug.h"
#include "Globals.h"
#include "Item.h"
#include "World.h"
#include "Events/ChangedInventoryItems.h"

#define INVENTORY_DEBUG_NAME "Inventory] "

#define MESSAGE_NOT_CARRYING_ANYTHING "  Not carrying anything.  "
#define MESSAGE_NOT_CARRYING_ANYTHING_EXCEPT_GOLD "  Not carrying anything except gold.  "
#define MESSAGE_STEALS " steals "
#define MESSAGE_STOLE " stole "

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
	static event::ChangedInventoryItems changed;
	static std::set<unsigned char> lost;

	static void setSlot(unsigned char key, const Item &item);
};
#endif
