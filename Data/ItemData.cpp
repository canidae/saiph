#include "ItemData.h"

using namespace std;

/* initialize static variables */
map<string, ItemData*> ItemData::items;

ItemData::ItemData(const string &name, int base_cost, int weight, char item_class, int material, unsigned long long properties) : name(name), base_cost(base_cost), weight(weight), item_class(item_class), material(material), properties(properties) {
	items[name] = this;
}
