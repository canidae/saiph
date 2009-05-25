#include "MorningStar.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, MorningStar *> MorningStar::morning_stars;

MorningStar::MorningStar(const string &name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2) : Weapon(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

void MorningStar::addToMap(const string &name, MorningStar *morning_star) {
	MorningStar::morning_stars[name] = morning_star;
	Weapon::addToMap(name, morning_star);
}

void MorningStar::create(const string &name, int cost, int weight, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new MorningStar(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}

void MorningStar::init() {
	/* morning stars */
	create("morning star", 10, 120, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* artifact morning stars */
	create("Trollsbane", 200, 120, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_IRON, ')', PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, CHAOTIC | NEUTRAL | LAWFUL);
}
