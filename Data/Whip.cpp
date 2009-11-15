#include "Whip.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Whip*> Whip::_whips;

/* protected static methods */
Whip::Whip(const string& name, int cost, int weight, char type, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Whip::~Whip() {
}

/* public static methods */
void Whip::init() {
	/* whips */
	create("bullwhip", 4, 20, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), Attack(), MATERIAL_LEATHER, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("rubber hose", 3, 20, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), MATERIAL_PLASTIC, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
}

const map<const string, const Whip*>& Whip::whips() {
	return _whips;
}

/* protected static methods */
void Whip::addToMap(const string& name, const Whip* whip) {
	_whips[name] = whip;
	Weapon::addToMap(name, whip);
}

/* private static methods */
void Whip::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char type, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Whip(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
