#include "Knife.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Knife*> Knife::_knives;

/* protected constructors */
Knife::Knife(const string& name, int cost, int weight, char type, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Knife::~Knife() {
}

/* public static methods */
void Knife::init() {
	/* knives */
	create("worm tooth", 2, 20, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_BONE, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("knife", 4, 5, Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("stiletto", 4, 5, Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("scalpel", 4, 5, Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), MATERIAL_METAL, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("crysknife", 100, 20, Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), Attack(), MATERIAL_MINERAL, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
}

const map<const string, const Knife*>& Knife::knives() {
	return _knives;
}

/* protected static methods */
void Knife::addToMap(const string& name, const Knife* knife) {
	_knives[name] = knife;
	Weapon::addToMap(name, knife);
}

/* private static methods */
void Knife::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char type, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Knife(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
