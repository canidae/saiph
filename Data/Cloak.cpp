#include "Cloak.h"

#include "../Globals.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Cloak*> Cloak::_cloaks;

/* protected constructors */
Cloak::Cloak(const string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : Armor(name, cost, weight, material, SLOT_CLOAK, ac, mc, properties) {
}

/* destructor */
Cloak::~Cloak() {
}

/* public static methods */
void Cloak::init() {
	/* cloaks */
	create("mummy wrapping", 2, 3, MATERIAL_CLOTH, 0, 1, PROPERTY_VISIBLE);
	create("orcish cloak", 40, 10, MATERIAL_CLOTH, 0, 2, 0);
	create("dwarvish cloak", 50, 10, MATERIAL_CLOTH, 0, 2, 0);
	create("leather cloak", 40, 15, MATERIAL_LEATHER, 1, 1, 0);
	create("oilskin cloak", 50, 10, MATERIAL_CLOTH, 1, 3, PROPERTY_SLIPPERY);
	create("alchemy smock", 50, 10, MATERIAL_CLOTH, 1, 1, PROPERTY_POISON | PROPERTY_ACID | PROPERTY_MAGIC);
	create("elven cloak", 60, 10, MATERIAL_CLOTH, 1, 3, PROPERTY_STEALTH | PROPERTY_MAGIC);
	create("robe", 50, 15, MATERIAL_CLOTH, 2, 3, PROPERTY_CASTING_BONUS | PROPERTY_MAGIC);
	create("cloak of magic resistance", 60, 10, MATERIAL_CLOTH, 1, 3, PROPERTY_MAGICRES | PROPERTY_MAGIC);
	create("cloak of displacement", 50, 10, MATERIAL_CLOTH, 1, 2, PROPERTY_DISPLACEMENT | PROPERTY_MAGIC);
	create("cloak of invisibility", 60, 10, MATERIAL_CLOTH, 1, 2, PROPERTY_INVISIBLE | PROPERTY_MAGIC);
	create("cloak of protection", 50, 10, MATERIAL_CLOTH, 3, 3, PROPERTY_MAGIC);

	/* aliases */
	addToMap("coarse mantelet", _cloaks["orcish cloak"]);
	addToMap("hooded cloak", _cloaks["dwarvish cloak"]);
	addToMap("slippery cloak", _cloaks["oilskin cloak"]);
	addToMap("apron", _cloaks["alchemy smock"]);
	addToMap("faded pall", _cloaks["elven cloak"]);

	/* TODO: randomized appearance */
	// tattered cape, ornamental cope, opera cloak, piece of cloth
}

const map<const string, const Cloak*> Cloak::cloaks() {
	return _cloaks;
}

/* protected static methods */
void Cloak::addToMap(const string& name, const Cloak* cloak) {
	_cloaks[name] = cloak;
	Armor::addToMap(name, cloak);
}

/* private static methods */
void Cloak::create(const string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
	addToMap(name, new Cloak(name, cost, weight, material, ac, mc, properties));
}
