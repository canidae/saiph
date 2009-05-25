#include "LongSword.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, LongSword *> LongSword::long_swords;

LongSword::LongSword(const string &name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2) : Weapon(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

void LongSword::addToMap(const string &name, LongSword *long_sword) {
	LongSword::long_swords[name] = long_sword;
	Weapon::addToMap(name, long_sword);
}

void LongSword::create(const string &name, int cost, int weight, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new LongSword(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}

void LongSword::init() {
	/* long swords */
	create("long sword", 15, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("katana", 80, 40, Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("samurai sword", long_swords["katana"]);

	/* artifact long swords */
	create("Demonbane", 2500, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, LAWFUL);
	create("Excalibur", 4000, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), MATERIAL_IRON, ')', PROPERTY_MAGIC | PROPERTY_SEARCHING | PROPERTY_ARTIFACT, true, LAWFUL);
	create("Fire Brand", 3000, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_FIRE, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(AT_CLAW, AD_FIRE, 1, 12), Attack(), MATERIAL_IRON, ')', PROPERTY_MAGIC | PROPERTY_FIRE | PROPERTY_ARTIFACT, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("Frost Brand", 3000, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_COLD, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(AT_CLAW, AD_COLD, 1, 12), Attack(), MATERIAL_IRON, ')', PROPERTY_MAGIC | PROPERTY_COLD | PROPERTY_ARTIFACT, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("Giantslayer", 200, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), MATERIAL_IRON, ')', PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("Snickersnee", 1200, 40, Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_IRON, ')', PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, LAWFUL);
	create("Sunsword", 1500, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), MATERIAL_IRON, ')', PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, LAWFUL);
	create("Vorpal Blade", 4000, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), MATERIAL_IRON, ')', PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, NEUTRAL);
}
