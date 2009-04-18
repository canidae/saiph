#ifndef _INVENTORY_H
#define _INVENTORY_H

#include <map>
#include <string>
#include "Debug.h"
#include "Item.h"
#include "World.h"

#define INVENTORY_DEBUG_NAME "Inventor] "

class Inventory {
public:
	static std::map<unsigned char, Item> items;

	Inventory();
	~Inventory();

	static void addItem(unsigned char key, const Item &item);
	static void parseMessages(const std::string &messages);
	static void removeItem(unsigned char key, const Item &item);
};

/* inline methods */
inline void Inventory::addItem(unsigned char key, const Item &item) {
	if (item.count <= 0)
		return;
	Debug::notice() << INVENTORY_DEBUG_NAME << "Adding " << item.count << " " << item.name << " to inventory slot " << key << std::endl;
	if (items.find(key) != items.end()) {
		/* existing item, add amount */
		items[key].count += item.count;
	} else {
		/* new item */
		items[key] = item;
	}
}

inline void Inventory::removeItem(unsigned char key, const Item &item) {
	if (item.count <= 0)
		return;
	std::map<unsigned char, Item>::iterator i = items.find(key);
	if (i == items.end())
		return;
	Debug::notice() << INVENTORY_DEBUG_NAME << "Removing " << item.count << " " << item.name << " from inventory slot " << key << std::endl;
	if (i->second.count > item.count)
		i->second.count -= item.count; // we got more than we remove
	else    
		items.erase(i); // removing all we got
}
#endif
