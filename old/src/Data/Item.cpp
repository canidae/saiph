#include "Data/Item.h"

#include <set>
#include "Data/Amulet.h"
#include "Data/Armor.h"
#include "Data/Food.h"
#include "Data/Tool.h"
#include "Data/Wand.h"
#include "Data/Weapon.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Item*> Item::_items;

/* protected constructors */
Item::Item(const string& name, int cost, int weight, char category, int material, unsigned long long properties) : _name(name), _cost(cost), _weight(weight), _category(category), _material(material), _properties(properties) {
}

/* destructor */
Item::~Item() {
}

/* public static methods */
void Item::init() {
	Amulet::init();
	Armor::init();
	Food::init();
	Tool::init();
	Wand::init();
	Weapon::init();
}

void Item::destroy() {
	/* we might have two mappings for the same Item (e.g., "low boots" and "walking shoes" are the same item).
	 * only free the assigned memore once */
	set<const Item*> unique_pointers;
	for (map<const string, const Item*>::iterator i = _items.begin(); i != _items.end(); ++i)
		unique_pointers.insert(i->second);
	for (set<const Item*>::iterator p = unique_pointers.begin(); p != unique_pointers.end(); ++p)
		delete *p;
	_items.clear();
}

const map<const string, const Item*>& Item::items() {
	return _items;
}

/* public methods */
const string& Item::name() const {
	return _name;
}

int Item::cost() const {
	return _cost;
}

int Item::weight() const {
	return _weight;
}

char Item::category() const {
	return _category;
}

int Item::material() const {
	return _material;
}

unsigned long long Item::properties() const {
	return _properties;
}

/* protected methods */
void Item::addToMap(const string& name, const Item* item) {
	_items[name] = item;
}
