#include "ShortSword.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, ShortSword*> ShortSword::short_swords;

ShortSword::ShortSword(const string& name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

void ShortSword::addToMap(const string& name, ShortSword* short_sword) {
	ShortSword::short_swords[name] = short_sword;
	Weapon::addToMap(name, short_sword);
}

void ShortSword::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new ShortSword(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}

void ShortSword::init() {
	/* short swords */
	create("orcish short sword", 10, 30, Attack(AT_CLAW, AD_PHYS, 1, 5), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("short sword", 10, 30, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("dwarvish short sword", 10, 30, Attack(AT_CLAW, AD_PHYS, 1, 7), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("elven short sword", 10, 30, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_WOOD, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("crude short sword", short_swords["orcish short sword"]);
	addToMap("broad short sword", short_swords["dwarvish short sword"]);
	addToMap("runed short sword", short_swords["elven short sword"]);
}
