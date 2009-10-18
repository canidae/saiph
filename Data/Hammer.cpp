#include "Hammer.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Hammer*> Hammer::hammers;

Hammer::Hammer(const string& name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

void Hammer::addToMap(const string& name, Hammer* hammer) {
	Hammer::hammers[name] = hammer;
	Weapon::addToMap(name, hammer);
}

void Hammer::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Hammer(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}

void Hammer::init() {
	/* hammers */
	create("war hammer", 5, 50, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* artifact hammers */
	create("Mjollnir", 4000, 50, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(AT_CLAW, AD_ELEC, 1, 24), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_ELEC, 1, 24), Attack(), MATERIAL_IRON, ')', PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, LAWFUL);
	create("Ogresmasher", 200, 50, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(), MATERIAL_IRON, ')', PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, CHAOTIC | NEUTRAL | LAWFUL);
}
