#include "Flail.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Flail *> Flail::flails;

Flail::Flail(const string &name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2) : Weapon(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

void Flail::addToMap(const string &name, Flail *flail) {
	Flail::flails[name] = flail;
	Weapon::addToMap(name, flail);
}

void Flail::create(const string &name, int cost, int weight, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Flail(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}

void Flail::init() {
	/* flails */
	create("flail", 4, 15, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("grappling hook", 50, 30, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_IRON, '(', 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("iron hook", flails["grappling hook"]);
}
