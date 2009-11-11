#include "Shirt.h"

#include "../Globals.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Shirt*> Shirt::_shirts;

/* protected constructors */
Shirt::Shirt(const string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : Armor(name, cost, weight, material, SLOT_SHIRT, ac, mc, properties) {
}

/* destructor */
Shirt::~Shirt() {
}

/* public static methods */
void Shirt::init() {
	/* shirts */
	create("Hawaiian shirt", 3, 5, MATERIAL_CLOTH, 0, 0, 0);
	create("T-shirt", 2, 5, MATERIAL_CLOTH, 0, 0, 0);
}

const map<const string, const Shirt*>& Shirt::shirts() {
	return _shirts;
}

/* protected static methods */
void Shirt::addToMap(const string& name, const Shirt* shirt) {
	_shirts[name] = shirt;
	Armor::addToMap(name, shirt);
}

/* private static methods */
void Shirt::create(const string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
	addToMap(name, new Shirt(name, cost, weight, material, ac, mc, properties));
}
