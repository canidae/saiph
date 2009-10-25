#include "Item.h"

#include <set>
#include "Amulet.h"
#include "Armor.h"
#include "Food.h"
#include "Tool.h"
#include "Weapon.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Item*> Item::_items;

/* protected constructors */
Item::Item(const string& name, const int& cost, const int& weight, const char& type, const int& material, const unsigned long long& properties) : _name(name), _cost(cost), _weight(weight), _type(type), _material(material), _properties(properties) {
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

const int& Item::cost() const {
	return _cost;
}

const int& Item::weight() const {
	return _weight;
}

const char& Item::type() const {
	return _type;
}

const int& Item::material() const {
	return _material;
}

const unsigned long long& Item::properties() const {
	return _properties;
}

/* protected methods */
void Item::addToMap(const string& name, const Item* item) {
	_items[name] = item;
}
