#include "CrossbowAmmo.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const CrossbowAmmo*> CrossbowAmmo::_crossbows;

/* protected constructors */
CrossbowAmmo::CrossbowAmmo(const string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, category, material, properties, one_handed, alignment, WEAPON_CROSSBOW_AMMO, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
CrossbowAmmo::~CrossbowAmmo() {
}

/* public static methods */
void CrossbowAmmo::init() {
	/* crossbow ammunition */
	create("crossbow bolt", 2, 1, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("poisoned crossbow bolt", _crossbows["crossbow bolt"]);
}

const map<const string, const CrossbowAmmo*>& CrossbowAmmo::crossbows() {
	return _crossbows;
}

/* protected static methods */
void CrossbowAmmo::addToMap(const string& name, const CrossbowAmmo* crossbow) {
	_crossbows[name] = crossbow;
	Weapon::addToMap(name, crossbow);
}

/* private static methods */
void CrossbowAmmo::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new CrossbowAmmo(name, cost, weight, category, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}