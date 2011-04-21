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
	static const Item NO_ITEM;
	
	static void analyze();
	static void parseMessages(const std::string& messages);
	static std::map<unsigned char, Item>& items() { return _items; }
	static const Item& itemAtKey(unsigned char key);
	static const Item& itemInSlot(int slot);
	static unsigned char keyForSlot(int slot);
	static int slotForKey(unsigned char key);
	static unsigned long long extrinsicsFromItems();
	static bool updated() { return _updated; }
	static void update();

private:
	static bool _updated;
	static std::map<unsigned char, Item> _items;
	static unsigned char _slots[SLOTS];
	static event::ChangedInventoryItems _changed;
	static std::set<unsigned char> _lost;
	static unsigned long long _extrinsics_from_items;
	static bool _extrinsics_updated;

	static void addItem(unsigned char key, const Item& item);
	static void removeItem(unsigned char key);
	static void setSlot(unsigned char key, const Item& item);
	static void updateExtrinsics();
};
#endif
