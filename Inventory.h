#ifndef _INVENTORY_H
#define _INVENTORY_H

#include <map>
#include <string>
#include "Item.h"

class Inventory {
public:
	static std::map<unsigned char, Item> items;

	Inventory();
	~Inventory();

	static void parseMessages(const std::string &messages);
};
#endif
