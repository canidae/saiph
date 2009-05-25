#include "Spear.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Spear *> Spear::spears;

Spear::Spear(const string &name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2) : Weapon(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

void Spear::addToMap(const string &name, Spear *spear) {
	Spear::spears[name] = spear;
	Weapon::addToMap(name, spear);
}

void Spear::create(const string &name, int cost, int weight, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Spear(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}

void Spear::init() {
	/* spears */
	create("orcish spear", 3, 30, Attack(AT_CLAW, AD_PHYS, 1, 5), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("spear", 3, 30, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("silver spear", 40, 36, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_SILVER, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("elven spear", 3, 30, Attack(AT_CLAW, AD_PHYS, 1, 7), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_WOOD, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("dwarvish spear", 3, 35, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("crude spear", spears["orcish spear"]);
	addToMap("runed spear", spears["elven spear"]);
	addToMap("stout spear", spears["dwarvish spear"]);
}
