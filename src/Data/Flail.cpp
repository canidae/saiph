#include "Data/Flail.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Flail*> Flail::_flails;

/* protected constructors */
Flail::Flail(const string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, category, material, properties, one_handed, alignment, WEAPON_FLAIL, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Flail::~Flail() {
}

/* public static methods */
void Flail::init() {
	/* flails */
	create("flail", 4, 15, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("grappling hook", 50, 30, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_IRON, TOOL, 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("iron hook", _flails["grappling hook"]);
}

const map<const string, const Flail*>& Flail::flails() {
	return _flails;
}

/* protected static methods */
void Flail::addToMap(const string& name, const Flail* flail) {
	_flails[name] = flail;
	Weapon::addToMap(name, flail);
}

/* private static methods */
void Flail::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Flail(name, cost, weight, category, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
