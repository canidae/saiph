#ifndef ITEMDATA_H
#define ITEMDATA_H

#include <string>

/* TODO: material? */

class ItemData {
public:
	ItemData(const std::string& name, int base_cost, int weight, bool magic, char item_class);
	const std::string name;
	const int base_cost;
	const int weight;
	const bool magic;
	const char item_class;
};

#endif ITEMDATA_H