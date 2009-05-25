#include "Club.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Club *> Club::clubs;

Club::Club(const string &name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2) : Weapon(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

void Club::addToMap(const string &name, Club *club) {
	Club::clubs[name] = club;
	Weapon::addToMap(name, club);
}

void Club::create(const string &name, int cost, int weight, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Club(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}

void Club::init() {
	/* clubs */
	create("club", 3, 30, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), MATERIAL_WOOD, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("aklys", 4, 15, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("thonged club", clubs["aklys"]);
}
