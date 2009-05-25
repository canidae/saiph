#include "Boomerang.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Boomerang *> Boomerang::boomerangs;

Boomerang::Boomerang(const string &name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2) : Weapon(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

void Boomerang::addToMap(const string &name, Boomerang *boomerang) {
	Boomerang::boomerangs[name] = boomerang;
	Weapon::addToMap(name, boomerang);
}

void Boomerang::create(const string &name, int cost, int weight, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Boomerang(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}

void Boomerang::init() {
	/* boomerangs */
	create("boomerang", 20, 5, Attack(AT_CLAW, AD_PHYS, 1, 9), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 9), Attack(), Attack(), MATERIAL_WOOD, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
}
