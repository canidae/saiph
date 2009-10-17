#include "Gloves.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Gloves *> Gloves::gloves;

Gloves::Gloves(const string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : Armor(name, cost, weight, material, SLOT_GLOVES, ac, mc, properties) {
}

void Gloves::addToMap(const string &name, Gloves *gloves) {
	Gloves::gloves[name] = gloves;
	Armor::addToMap(name, gloves);
}

void Gloves::create(const string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
	addToMap(name, new Gloves(name, cost, weight, material, ac, mc, properties));
}

void Gloves::init() {
	/* gloves */
	create("leather gloves", 8, 10, MATERIAL_LEATHER, 1, 0, 0);
	create("gauntlets of dexterity", 50, 10, MATERIAL_LEATHER, 1, 0, PROPERTY_DEXTERITY);
	create("gauntlets of power", 50, 30, MATERIAL_IRON, 1, 0, PROPERTY_STRENGTH);
	create("gauntlets of fumbling", 50, 10, MATERIAL_LEATHER, 1, 0, PROPERTY_FUMBLING);
}
