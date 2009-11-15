#include "Scimitar.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Scimitar*> Scimitar::_scimitars;

/* protected constructors */
Scimitar::Scimitar(const string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, category, material, properties, one_handed, alignment, WEAPON_SCIMITAR, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Scimitar::~Scimitar() {
}

/* public static methods */
void Scimitar::init() {
	/* scimitars */
	create("scimitar", 15, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("curved sword", _scimitars["scimitar"]);
}

const map<const string, const Scimitar*>& Scimitar::scimitars() {
	return _scimitars;
}

/* protected static methods */
void Scimitar::addToMap(const string& name, const Scimitar* scimitar) {
	_scimitars[name] = scimitar;
	Weapon::addToMap(name, scimitar);
}

/* private static methods */
void Scimitar::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Scimitar(name, cost, weight, category, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
