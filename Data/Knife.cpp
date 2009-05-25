#include "Knife.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Knife *> Knife::knives;

Knife::Knife(const string &name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2) : Weapon(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

void Knife::addToMap(const string &name, Knife *knife) {
	Knife::knives[name] = knife;
	Weapon::addToMap(name, knife);
}

void Knife::create(const string &name, int cost, int weight, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Knife(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}

void Knife::init() {
	/* knives */
	create("worm tooth", 2, 20, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_BONE, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("knife", 4, 5, Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("stiletto", 4, 5, Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("scalpel", 4, 5, Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), MATERIAL_METAL, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("crysknife", 100, 20, Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), Attack(), MATERIAL_MINERAL, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
}
