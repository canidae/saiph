#include "Amulet.h"
#include "Armor.h"
#include "Food.h"
#include "Item.h"

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
	Amulet::init();
	Armor::init();
	Food::init();
}

void Item::destroy() {
	/* we might have two mappings for the same Item (e.g., "low boots" and "walking shoes" are the same item).
	 * only free the assigned memore once */
	set<Item *> unique_pointers;
	for (map<string, Item *>::iterator i = items.begin(); i != items.end(); ++i)
		unique_pointers.insert(i->second);
	for (set<Item *>::iterator p = unique_pointers.begin(); p != unique_pointers.end(); ++p)
		delete *p;
	items.clear();
}
