#include "Shirt.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Shirt*> Shirt::shirts;

Shirt::Shirt(const string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : Armor(name, cost, weight, material, SLOT_SHIRT, ac, mc, properties) {
}

void Shirt::addToMap(const string& name, Shirt* shirt) {
	Shirt::shirts[name] = shirt;
	Armor::addToMap(name, shirt);
}

void Shirt::create(const string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
	addToMap(name, new Shirt(name, cost, weight, material, ac, mc, properties));
}

void Shirt::init() {
	/* shirts */
	create("Hawaiian shirt", 3, 5, MATERIAL_CLOTH, 0, 0, 0);
	create("T-shirt", 2, 5, MATERIAL_CLOTH, 0, 0, 0);
}
