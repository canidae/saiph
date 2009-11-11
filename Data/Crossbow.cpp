#include "Crossbow.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Crossbow*> Crossbow::_crossbows;

/* protected constructors */
Crossbow::Crossbow(const string& name, int cost, int weight, char type, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Crossbow::~Crossbow() {
}

/* public static methods */
void Crossbow::init() {
	/* crossbows */
	create("crossbow", 40, 50, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_WOOD, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("crossbow bolt", 2, 1, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
}

const map<const string, const Crossbow*>& Crossbow::crossbows() {
	return _crossbows;
}

/* protected static methods */
void Crossbow::addToMap(const string& name, const Crossbow* crossbow) {
	_crossbows[name] = crossbow;
	Weapon::addToMap(name, crossbow);
}

/* private static methods */
void Crossbow::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char type, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Crossbow(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
