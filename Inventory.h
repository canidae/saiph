#ifndef INVENTORY_H
#define INVENTORY_H

#include <map>
#include <set>
#include <string>
#include "Globals.h"
#include "Item.h"
#include "Events/ChangedInventoryItems.h"

class Inventory {
public:
	static void analyze();
	static void parseMessages(const std::string& messages);
	static std::map<unsigned char, Item>& items();
	static bool updated();
	static bool updated(bool updated);
	static unsigned char itemInSlot(int slot);
	static void addItem(unsigned char key, const Item& item);
	static void removeItem(unsigned char key, const Item& item);

private:
	static bool _updated;
	static std::map<unsigned char, Item> _items;
	static unsigned char _slots[SLOTS];
	static event::ChangedInventoryItems _changed;
	static std::set<unsigned char> _lost;

	static void setSlot(unsigned char key, const Item& item);
};
#endif
