#include "Data/Helmet.h"

#include "Globals.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Helmet*> Helmet::_helmets;

/* protected constructors */
Helmet::Helmet(const string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : Armor(name, cost, weight, material, SLOT_HELMET, ac, mc, properties) {
}

/* destructor */
Helmet::~Helmet() {
}

/* public static methods */
void Helmet::init() {
	/* helms */
	create("helm of brilliance", 50, 50, MATERIAL_IRON, 1, 0, PROPERTY_MAGIC | PROPERTY_BRILLIANCE);
	create("helm of opposite alignment", 50, 50, MATERIAL_IRON, 1, 0, PROPERTY_MAGIC);
	create("dunce cap", 1, 4, MATERIAL_CLOTH, 0, 0, PROPERTY_MAGIC | PROPERTY_STUPIDITY);
	create("cornuthaum", 80, 4, MATERIAL_CLOTH, 0, 2, PROPERTY_MAGIC);
	create("helm of telepathy", 50, 50, MATERIAL_IRON, 1, 0, PROPERTY_MAGIC | PROPERTY_ESP);
	create("dwarvish iron helm", 20, 40, MATERIAL_IRON, 2, 0, 0);
	create("Mitre of Holiness", 2000, 50, MATERIAL_IRON, 1, 0, PROPERTY_MAGIC | PROPERTY_BRILLIANCE);
	create("helmet", 10, 30, MATERIAL_IRON, 1, 0, 0);
	create("dented pot", 8, 10, MATERIAL_IRON, 1, 0, 0);
	create("elven leather helm", 8, 3, MATERIAL_LEATHER, 1, 0, 0);
	create("fedora", 1, 3, MATERIAL_CLOTH, 0, 0, 0);
	create("orcish helm", 10, 30, MATERIAL_IRON, 1, 0, 0);

	/* random appearances */
	create("plumed helmet", 50, 50, MATERIAL_IRON, 1, 0, PROPERTY_RANDOM_APPEARANCE);
	create("etched helmet", 50, 50, MATERIAL_IRON, 1, 0, PROPERTY_RANDOM_APPEARANCE);
	create("crested helmet", 50, 50, MATERIAL_IRON, 1, 0, PROPERTY_RANDOM_APPEARANCE);
	create("visored helmet", 50, 50, MATERIAL_IRON, 1, 0, PROPERTY_RANDOM_APPEARANCE);

	/* aliases */
	addToMap("iron skull cap", _helmets["orcish helm"]);
	addToMap("conical hat", _helmets["cornuthaum"]);
	addToMap("leather hat", _helmets["elven leather helm"]);
	addToMap("hard hat", _helmets["dwarvish iron helm"]);
}

const map<const string, const Helmet*>& Helmet::helmets() {
	return _helmets;
}

/* protected static methods */
void Helmet::addToMap(const string& name, const Helmet* helmet) {
	_helmets[name] = helmet;
	Armor::addToMap(name, helmet);
}

/* private static methods */
void Helmet::create(const string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
	addToMap(name, new Helmet(name, cost, weight, material, ac, mc, properties));
}
