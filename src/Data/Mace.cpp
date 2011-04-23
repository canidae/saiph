#include "Data/Mace.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Mace*> Mace::_maces;

/* protected constructors */
Mace::Mace(const string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, category, material, properties, one_handed, alignment, P_MACE, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Mace::~Mace() {
}

/* public static methods */
void Mace::init() {
	/* maces */
	create("mace", 5, 30, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* artifact maces */
	create("The Sceptre of Might", 2500, 30, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), MATERIAL_IRON, WEAPON, PROPERTY_MAGICRES | PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, LAWFUL);
}

const map<const string, const Mace*>& Mace::maces() {
	return _maces;
}

/* protected static methods */
void Mace::addToMap(const string& name, const Mace* mace) {
	_maces[name] = mace;
	Weapon::addToMap(name, mace);
}

/* private static methods */
void Mace::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Mace(name, cost, weight, category, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
