#include "Boomerang.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Boomerang*> Boomerang::_boomerangs;

/* protected constructors */
Boomerang::Boomerang(const string& name, int cost, int weight, char type, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Boomerang::~Boomerang() {
}

/* public static methods */
void Boomerang::init() {
	/* boomerangs */
	create("boomerang", 20, 5, Attack(AT_CLAW, AD_PHYS, 1, 9), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 9), Attack(), Attack(), MATERIAL_WOOD, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
}

const map<const string, const Boomerang*>& Boomerang::boomerangs() {
	return _boomerangs;
}

/* protected static methods */
void Boomerang::addToMap(const string& name, const Boomerang* boomerang) {
	_boomerangs[name] = boomerang;
	Weapon::addToMap(name, boomerang);
}

/* private static methods */
void Boomerang::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char type, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Boomerang(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
