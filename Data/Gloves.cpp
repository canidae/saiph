#include "Gloves.h"

#include "../Globals.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Gloves*> Gloves::_gloves;

/* protected constructors */
Gloves::Gloves(const string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : Armor(name, cost, weight, material, SLOT_GLOVES, ac, mc, properties) {
}

/* destructor */
Gloves::~Gloves() {
}

/* public static methods */
void Gloves::init() {
	/* gloves */
	create("leather gloves", 8, 10, MATERIAL_LEATHER, 1, 0, 0);
	create("gauntlets of dexterity", 50, 10, MATERIAL_LEATHER, 1, 0, PROPERTY_DEXTERITY);
	create("gauntlets of power", 50, 30, MATERIAL_IRON, 1, 0, PROPERTY_STRENGTH);
	create("gauntlets of fumbling", 50, 10, MATERIAL_LEATHER, 1, 0, PROPERTY_FUMBLING);
}

const map<const string, const Gloves*>& Gloves::gloves() {
	return _gloves;
}

/* protected static methods */
void Gloves::addToMap(const string& name, const Gloves* gloves) {
	_gloves[name] = gloves;
	Armor::addToMap(name, gloves);
}

/* private static methods */
void Gloves::create(const string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
	addToMap(name, new Gloves(name, cost, weight, material, ac, mc, properties));
}
