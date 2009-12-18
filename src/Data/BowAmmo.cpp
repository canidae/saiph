#include "Data/BowAmmo.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const BowAmmo*> BowAmmo::_bows;

/* protected constructors */
BowAmmo::BowAmmo(const string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, category, material, properties, one_handed, alignment, WEAPON_BOW_AMMO, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
BowAmmo::~BowAmmo() {
}

/* public static methods */
void BowAmmo::init() {
	/* bow ammunition */
	create("orcish arrow", 2, 1, Attack(AT_CLAW, AD_PHYS, 1, 5), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("arrow", 2, 1, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("elven arrow", 2, 1, Attack(AT_CLAW, AD_PHYS, 1, 7), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_WOOD, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("ya", 4, 1, Attack(AT_CLAW, AD_PHYS, 1, 7), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 7), Attack(), Attack(), MATERIAL_METAL, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("silver arrow", 5, 1, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_SILVER, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("crude arrow", _bows["orcish arrow"]);
	addToMap("runed arrow", _bows["elven arrow"]);
	addToMap("bamboo arrow", _bows["ya"]);
	addToMap("poisoned crude arrow", _bows["orcish arrow"]);
	addToMap("poisoned runed arrow", _bows["elven arrow"]);
	addToMap("poisoned bamboo arrow", _bows["ya"]);
	addToMap("poisoned arrow", _bows["arrow"]);
	addToMap("poisoned orcish arrow", _bows["orcish arrow"]);
	addToMap("poisoned elven arrow", _bows["elven arrow"]);
	addToMap("poisoned ya", _bows["ya"]);
	addToMap("poisoned silver arrow", _bows["silver arrow"]);
}

const map<const string, const BowAmmo*>& BowAmmo::bows() {
	return _bows;
}

/* protected static methods */
void BowAmmo::addToMap(const string& name, const BowAmmo* bow) {
	_bows[name] = bow;
	Weapon::addToMap(name, bow);
}

/* private static methods */
void BowAmmo::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new BowAmmo(name, cost, weight, category, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
