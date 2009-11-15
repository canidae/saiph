#include "Spear.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Spear*> Spear::_spears;

/* protected constructors */
Spear::Spear(const string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, category, material, properties, one_handed, alignment, WEAPON_SPEAR, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Spear::~Spear() {
}

/* public static methods */
void Spear::init() {
	/* spears */
	create("orcish spear", 3, 30, Attack(AT_CLAW, AD_PHYS, 1, 5), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("spear", 3, 30, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("silver spear", 40, 36, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_SILVER, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("elven spear", 3, 30, Attack(AT_CLAW, AD_PHYS, 1, 7), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_WOOD, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("dwarvish spear", 3, 35, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("crude spear", _spears["orcish spear"]);
	addToMap("runed spear", _spears["elven spear"]);
	addToMap("stout spear", _spears["dwarvish spear"]);
}

const map<const string, const Spear*>& Spear::spears() {
	return _spears;
}

/* protected static methods */
void Spear::addToMap(const string& name, const Spear* spear) {
	_spears[name] = spear;
	Weapon::addToMap(name, spear);
}

/* private static methods */
void Spear::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Spear(name, cost, weight, category, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
