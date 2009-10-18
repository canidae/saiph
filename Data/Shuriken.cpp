#include "Shuriken.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Shuriken*> Shuriken::shurikens;

Shuriken::Shuriken(const string& name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

void Shuriken::addToMap(const string& name, Shuriken* shuriken) {
	Shuriken::shurikens[name] = shuriken;
	Weapon::addToMap(name, shuriken);
}

void Shuriken::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Shuriken(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}

void Shuriken::init() {
	/* shurikens */
	create("shuriken", 5, 1, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("throwing star", shurikens["shuriken"]);
}
