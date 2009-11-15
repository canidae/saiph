#include "Axe.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Axe*> Axe::_axes;

/* protected constructors */
Axe::Axe(const string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, category, material, properties, one_handed, alignment, WEAPON_AXE, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Axe::~Axe() {
}

/* public static methods */
void Axe::init() {
	/* axes */
	create("axe", 8, 60, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("battle-axe", 40, 120, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), MATERIAL_IRON, WEAPON, 0, false, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("double-headed axe", _axes["battle-axe"]);

	/* artifact axes */
	create("Cleaver", 1500, 120, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(AT_CLAW, AD_PHYS, 1, 6), MATERIAL_IRON, WEAPON, PROPERTY_MAGIC | PROPERTY_ARTIFACT, false, NEUTRAL);
}

const map<const string, const Axe*>& Axe::axes() {
	return _axes;
}

/* protected static methods */
void Axe::addToMap(const string& name, const Axe* axe) {
	_axes[name] = axe;
	Weapon::addToMap(name, axe);
}

/* private static methods */
void Axe::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Axe(name, cost, weight, category, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}

