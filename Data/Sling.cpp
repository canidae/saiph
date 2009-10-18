#include "Sling.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Sling*> Sling::slings;

Sling::Sling(const string& name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

void Sling::addToMap(const string& name, Sling* sling) {
	Sling::slings[name] = sling;
	Weapon::addToMap(name, sling);
}

void Sling::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Sling(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}

void Sling::init() {
	/* slings */
	create("sling", 20, 3, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_LEATHER, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("flintstone", 1, 10, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_MINERAL, '*', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
}
