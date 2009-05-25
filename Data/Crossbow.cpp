#include "Crossbow.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Crossbow *> Crossbow::crossbows;

Crossbow::Crossbow(const string &name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2) : Weapon(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

void Crossbow::addToMap(const string &name, Crossbow *crossbow) {
	Crossbow::crossbows[name] = crossbow;
	Weapon::addToMap(name, crossbow);
}

void Crossbow::create(const string &name, int cost, int weight, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Crossbow(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}

void Crossbow::init() {
	/* crossbows */
	create("crossbow", 40, 50, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_WOOD, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("crossbow bolt", 2, 1, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
}
