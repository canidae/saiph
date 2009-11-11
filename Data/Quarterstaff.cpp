#include "Quarterstaff.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Quarterstaff*> Quarterstaff::_quarterstaves;

/* protected constructors */
Quarterstaff::Quarterstaff(const string& name, int cost, int weight, char type, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Quarterstaff::~Quarterstaff() {
}

/* public static methods */
void Quarterstaff::init() {
	/* quarterstaves */
	create("quarterstaff", 5, 40, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_WOOD, ')', 0, false, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("staff", _quarterstaves["quarterstaff"]);

	/* artifact quarterstaves */
	create("The Staff of Aesculapius", 5000, 40, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 8), MATERIAL_WOOD, ')', PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, LAWFUL);
}

const map<const string, const Quarterstaff*>& Quarterstaff::quarterstaves() {
	return _quarterstaves;
}

/* protected static methods */
void Quarterstaff::addToMap(const string& name, const Quarterstaff* quarterstaff) {
	_quarterstaves[name] = quarterstaff;
	Weapon::addToMap(name, quarterstaff);
}

/* private static methods */
void Quarterstaff::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char type, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Quarterstaff(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
