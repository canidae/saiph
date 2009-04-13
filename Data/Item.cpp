#include "Item.h"
#include "Armor.h"
#include "Food.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Item *> Item::items;

Item::Item(const string &name, int base_cost, int weight, char item_class, int material, unsigned long long properties) : name(name), base_cost(base_cost), weight(weight), item_class(item_class), material(material), properties(properties) {
}

void Item::addToMap(const string &name, Item *item) {
	Item::items[name] = item;
}

void Item::init() {
	Armor::init();
	Food::init();
}

void Item::destroy() {
	for (map<string, Item *>::iterator i = items.begin(); i != items.end(); ++i) {
		delete i->second;
		// we might have two mappings for the same Item (e.g., "low boots"
		// and "walking shoes" are the same item); make any double-deletes harmless
		i->second = NULL;
	}
}
