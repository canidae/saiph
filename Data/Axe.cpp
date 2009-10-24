#include "Axe.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Axe*> Axe::_axes;

/* constructors/destructor */
Axe::Axe(const string& name, const int& cost, const int& weight, const char& item_class, const int& material, const unsigned long long& properties, const bool& one_handed, const int& alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

Axe::~Axe() {
}

/* public static methods */
void Axe::init() {
	/* axes */
	create("axe", 8, 60, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("battle-axe", 40, 120, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), MATERIAL_IRON, ')', 0, false, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("double-headed axe", _axes["battle-axe"]);

	/* artifact axes */
	create("Cleaver", 1500, 120, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(AT_CLAW, AD_PHYS, 1, 6), MATERIAL_IRON, ')', PROPERTY_MAGIC | PROPERTY_ARTIFACT, false, NEUTRAL);
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
void Axe::create(const string& name, const int& cost, const int& weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, const int& material, const char& item_class, const unsigned long long& properties, const bool& one_handed, const int& alignment) {
	addToMap(name, new Axe(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}

