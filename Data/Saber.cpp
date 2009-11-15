#include "Saber.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Saber*> Saber::_sabers;

/* protected constructors */
Saber::Saber(const string& name, int cost, int weight, char type, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Saber::~Saber() {
}

/* public static methods */
void Saber::init() {
	/* sabers */
	create("silver saber", 75, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_SILVER, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* artifact sabers */
	create("Grayswandir", 8000, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_SILVER, WEAPON, PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, LAWFUL);
	create("Werebane", 1500, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_SILVER, WEAPON, PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, CHAOTIC | NEUTRAL | LAWFUL);
}

const map<const string, const Saber*>& Saber::sabers() {
	return _sabers;
}

/* protected static methods */
void Saber::addToMap(const string& name, const Saber* saber) {
	_sabers[name] = saber;
	Weapon::addToMap(name, saber);
}

/* private static methods */
void Saber::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char type, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Saber(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
