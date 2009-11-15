#include "Shuriken.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Shuriken*> Shuriken::_shurikens;

/* protected constructors */
Shuriken::Shuriken(const string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, category, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Shuriken::~Shuriken() {
}

/* public static methods */
void Shuriken::init() {
	/* shurikens */
	create("shuriken", 5, 1, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("throwing star", _shurikens["shuriken"]);
}

const map<const string, const Shuriken*>& Shuriken::shurikens() {
	return _shurikens;
}

/* protected static methods */
void Shuriken::addToMap(const string& name, const Shuriken* shuriken) {
	_shurikens[name] = shuriken;
	Weapon::addToMap(name, shuriken);
}

/* private static methods */
void Shuriken::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Shuriken(name, cost, weight, category, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
