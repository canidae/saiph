#include "Weapon.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Weapon *> Weapon::weapons;

Weapon::Weapon(const string &name, int cost, int weight, char item_class, int material, unsigned long long properties, unsigned long long properties_wielded, int alignment, bool one_handed, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2) : Item(name, cost, weight, item_class, material, properties), properties_wielded(properties_wielded), alignment(alignment), one_handed(one_handed) {
	attack_small[0] = as0;
	attack_small[1] = as1;
	attack_small[2] = as2;
	attack_large[0] = al0;
	attack_large[1] = al1;
	attack_large[2] = al2;
}

void Weapon::addToMap(const string &name, Weapon *weapon) {
	Weapon::weapons[name] = weapon;
	Item::addToMap(name, weapon);
}

void Weapon::create(const string &name, int cost, int weight, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2, int material, char item_class, unsigned long long properties, unsigned long long properties_wielded, int alignment, bool one_handed) {
	addToMap(name, new Weapon(name, cost, weight, item_class, material, properties, properties_wielded, alignment, one_handed, as0, as1, as2, al0, al1, al2));
}

void Weapon::createSimple(const string &name, int cost, int weight, const Attack &as0, const Attack &as1, const Attack &al0, const Attack &al1, int material, char item_class, bool one_handed) {
	create(name, cost, weight, as0, as1, Attack(), al0, al1, Attack(), material, item_class, 0, 0, CHAOTIC | NEUTRAL | LAWFUL, one_handed);
}

void Weapon::init() {
	/* weapons */
	createSimple("orcish dagger", 4, 10, Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), MATERIAL_IRON, ')', true);
	createSimple("dagger", 4, 10, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), MATERIAL_IRON, ')', true);
	createSimple("silver dagger", 40, 12, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), MATERIAL_SILVER, ')', true);
	createSimple("athame", 4, 10, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), MATERIAL_IRON, ')', true);
	createSimple("elven dagger", 4, 10, Attack(AT_CLAW, AD_PHYS, 1, 5), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), MATERIAL_WOOD, ')', true);
	createSimple("worm tooth", 2, 20, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), MATERIAL_BONE, ')', true);
	createSimple("knife", 4, 5, Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), MATERIAL_IRON, ')', true);
	createSimple("stiletto", 4, 5, Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), MATERIAL_IRON, ')', true);
	createSimple("scalpel", 4, 5, Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), MATERIAL_METAL, ')', true);
	createSimple("crysknife", 100, 20, Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), MATERIAL_MINERAL, ')', true);
	createSimple("axe", 8, 60, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), MATERIAL_IRON, ')', true);
	createSimple("battle-axe", 40, 120, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 2, 4), MATERIAL_IRON, ')', false);
	createSimple("pick-axe", 50, 100, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), MATERIAL_IRON, '(', true);
	createSimple("dwarvish mattock", 50, 120, Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 2, 6), MATERIAL_IRON, ')', false);
	createSimple("orcish short sword", 10, 30, Attack(AT_CLAW, AD_PHYS, 1, 5), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_IRON, ')', true);
	createSimple("short sword", 10, 30, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_IRON, ')', true);
	createSimple("dwarvish short sword", 10, 30, Attack(AT_CLAW, AD_PHYS, 1, 7), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_IRON, ')', true);
	createSimple("elven short sword", 10, 30, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_WOOD, ')', true);
	createSimple("broadsword", 10, 70, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), MATERIAL_IRON, ')', true);
	createSimple("runesword", 300, 40, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), MATERIAL_IRON, ')', true);
	createSimple("elven broadsword", 10, 70, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), MATERIAL_WOOD, ')', true);
	createSimple("long sword", 15, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), MATERIAL_IRON, ')', true);
	createSimple("katana", 80, 40, Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), MATERIAL_IRON, ')', true);
	createSimple("two-handed sword", 50, 150, Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(AT_CLAW, AD_PHYS, 3, 6), Attack(), MATERIAL_IRON, ')', false);
	createSimple("tsurugi", 500, 60, Attack(AT_CLAW, AD_PHYS, 1, 16), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 2, 6), MATERIAL_METAL, ')', false);
	createSimple("scimitar", 15, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_IRON, ')', true);
	createSimple("silver saber", 75, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_SILVER, ')', true);
	createSimple("club", 3, 30, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), MATERIAL_WOOD, ')', true);
	createSimple("aklys", 4, 15, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), MATERIAL_IRON, ')', true);
	createSimple("mace", 5, 30, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), MATERIAL_IRON, ')', true);
	createSimple("morning star", 10, 120, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), MATERIAL_IRON, ')', true);
	createSimple("flail", 4, 15, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), MATERIAL_IRON, ')', true);
	createSimple("grappling hook", 50, 30, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), MATERIAL_IRON, '(', true);
	createSimple("war hammer", 5, 50, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), MATERIAL_IRON, ')', true);
	createSimple("quarterstaff", 5, 40, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), MATERIAL_WOOD, ')', false);
	createSimple("partisan", 10, 80, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), MATERIAL_IRON, ')', false);
	createSimple("fauchard", 5, 60, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_IRON, ')', false);
	createSimple("glaive", 6, 75, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), MATERIAL_IRON, ')', false);
	createSimple("bec-de-corbin", 8, 100, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), MATERIAL_IRON, ')', false);
	createSimple("spetum", 5, 50, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(AT_CLAW, AD_PHYS, 2, 6), Attack(), MATERIAL_IRON, ')', false);
	createSimple("lucern hammer", 7, 150, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), MATERIAL_IRON, ')', false);
	createSimple("guisarme", 5, 80, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_IRON, ')', false);
	createSimple("ranseur", 6, 50, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), MATERIAL_IRON, ')', false);
	createSimple("voulge", 5, 125, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), MATERIAL_IRON, ')', false);
	createSimple("bill-guisarme", 7, 120, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), MATERIAL_IRON, ')', false);
	createSimple("bardiche", 7, 120, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 3, 4), Attack(), MATERIAL_IRON, ')', false);
	createSimple("halberd", 10, 150, Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), Attack(AT_CLAW, AD_PHYS, 2, 6), Attack(), MATERIAL_IRON, ')', false);
	createSimple("orcish spear", 3, 30, Attack(AT_CLAW, AD_PHYS, 1, 5), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_IRON, ')', true);
	createSimple("spear", 3, 30, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_IRON, ')', true);
	createSimple("silver spear", 40, 36, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_SILVER, ')', true);
	createSimple("elven spear", 3, 30, Attack(AT_CLAW, AD_PHYS, 1, 7), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_WOOD, ')', true);
	createSimple("dwarvish spear", 3, 35, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_IRON, ')', true);
	createSimple("javelin", 3, 20, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), MATERIAL_IRON, ')', true);
	createSimple("trident", 5, 25, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(AT_CLAW, AD_PHYS, 3, 4), Attack(), MATERIAL_IRON, ')', true);
	createSimple("lance", 10, 180, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_IRON, ')', true);
	createSimple("orcish bow", 60, 30, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), MATERIAL_WOOD, ')', true);
	createSimple("orcish arrow", 2, 1, Attack(AT_CLAW, AD_PHYS, 1, 5), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), MATERIAL_IRON, ')', true);
	createSimple("bow", 60, 30, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), MATERIAL_WOOD, ')', true);
	createSimple("arrow", 2, 1, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), MATERIAL_IRON, ')', true);
	createSimple("elven bow", 60, 30, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), MATERIAL_WOOD, ')', true);
	createSimple("elven arrow", 2, 1, Attack(AT_CLAW, AD_PHYS, 1, 7), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), MATERIAL_WOOD, ')', true);
	createSimple("yumi", 60, 30, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), MATERIAL_WOOD, ')', true);
	createSimple("ya", 4, 1, Attack(AT_CLAW, AD_PHYS, 1, 7), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 7), Attack(), MATERIAL_METAL, ')', true);
	createSimple("silver arrow", 5, 1, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), MATERIAL_SILVER, ')', true);
	createSimple("sling", 20, 3, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), MATERIAL_LEATHER, ')', true);
	createSimple("flintstone", 1, 10, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), MATERIAL_MINERAL, '*', true);
	createSimple("crossbow", 40, 50, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), MATERIAL_WOOD, ')', true);
	createSimple("crossbow bolt", 2, 1, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), MATERIAL_IRON, ')', true);
	createSimple("dart", 2, 1, Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), MATERIAL_IRON, ')', true);
	createSimple("shuriken", 5, 1, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), MATERIAL_IRON, ')', true);
	createSimple("boomerang", 20, 5, Attack(AT_CLAW, AD_PHYS, 1, 9), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 9), Attack(), MATERIAL_WOOD, ')', true);
	createSimple("bullwhip", 4, 20, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), MATERIAL_LEATHER, ')', true);
	createSimple("rubber hose", 3, 20, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), MATERIAL_PLASTIC, ')', true);
	createSimple("unicorn horn", 100, 20, Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), MATERIAL_BONE, '(', false);

	/* aliases */
	addToMap("crude dagger", weapons["orcish dagger"]);
	addToMap("runed dagger", weapons["elven dagger"]);
	addToMap("double-headed axe", weapons["battle-axe"]);
	addToMap("broad pick", weapons["dwarvish mattock"]);
	addToMap("crude short sword", weapons["orcish short sword"]);
	addToMap("broad short sword", weapons["dwarvish short sword"]);
	addToMap("runed short sword", weapons["elven short sword"]);
	addToMap("runed broadsword", weapons["runesword"]);
	addToMap("runed broadsword", weapons["elven broadsword"]);
	addToMap("samurai sword", weapons["katana"]);
	addToMap("long samurai sword", weapons["tsurugi"]);
	addToMap("curved sword", weapons["scimitar"]);
	addToMap("thonged club", weapons["aklys"]);
	addToMap("iron hook", weapons["grappling hook"]);
	addToMap("staff", weapons["quarterstaff"]);
	addToMap("vulgar polearm", weapons["partisan"]);
	addToMap("pole sickle", weapons["fauchard"]);
	addToMap("single-edged polearm", weapons["glaive"]);
	addToMap("beaked polearm", weapons["bec-de-corbin"]);
	addToMap("forked polearm", weapons["spetum"]);
	addToMap("pronged polearm", weapons["lucern hammer"]);
	addToMap("pruning hook", weapons["guisarme"]);
	addToMap("hilted polearm", weapons["ranseur"]);
	addToMap("pole cleaver", weapons["voulge"]);
	addToMap("hooked polearm", weapons["bill-guisarme"]);
	addToMap("long poleaxe", weapons["bardiche"]);
	addToMap("angled poleaxe", weapons["halberd"]);
	addToMap("crude spear", weapons["orcish spear"]);
	addToMap("runed spear", weapons["elven spear"]);
	addToMap("stout spear", weapons["dwarvish spear"]);
	addToMap("throwing spear", weapons["javelin"]);
	addToMap("crude bow", weapons["orcish bow"]);
	addToMap("crude arrow", weapons["orcish arrow"]);
	addToMap("runed bow", weapons["elven bow"]);
	addToMap("runed arrow", weapons["elven arrow"]);
	addToMap("long bow", weapons["yumi"]);
	addToMap("bamboo arrow", weapons["ya"]);
	addToMap("throwing star", weapons["shuriken"]);

	/* artifact weapons */
	create("Cleaver", 1500, 120, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(AT_CLAW, AD_PHYS, 1, 6), MATERIAL_IRON, ')', 0, 0, NEUTRAL, false);
	create("Demonbane", 2500, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), MATERIAL_IRON, ')', 0, 0, LAWFUL, true);
	create("Dragonbane", 500, 70, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), MATERIAL_IRON, ')', 0, 0, CHAOTIC | NEUTRAL | LAWFUL, true);
	create("Excalibur", 4000, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), MATERIAL_IRON, ')', 0, PROPERTY_SEARCHING, LAWFUL, true);
	create("Fire Brand", 3000, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_FIRE, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(AT_CLAW, AD_FIRE, 1, 12), Attack(), MATERIAL_IRON, ')', 0, PROPERTY_FIRE, CHAOTIC | NEUTRAL | LAWFUL, true);
	create("Frost Brand", 3000, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_COLD, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(AT_CLAW, AD_COLD, 1, 12), Attack(), MATERIAL_IRON, ')', 0, PROPERTY_COLD, CHAOTIC | NEUTRAL | LAWFUL, true);
	create("Giantslayer", 200, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), MATERIAL_IRON, ')', 0, 0, CHAOTIC | NEUTRAL | LAWFUL, true);
	create("Grayswandir", 8000, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_SILVER, ')', 0, 0, LAWFUL, true);
	create("Grimtooth", 300, 10, Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), MATERIAL_IRON, ')', 0, 0, CHAOTIC, true);
	create("The Longbow of Diana", 4000, 30, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_WOOD, ')', PROPERTY_ESP, PROPERTY_REFLECTION, CHAOTIC, true);
	create("Magicbane", 3500, 10, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), MATERIAL_IRON, ')', 0, PROPERTY_MAGICRES, NEUTRAL, true);
	create("Mjollnir", 4000, 50, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(AT_CLAW, AD_ELEC, 1, 24), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_ELEC, 1, 24), Attack(), MATERIAL_IRON, ')', 0, 0, LAWFUL, true);
	create("Ogresmasher", 200, 50, Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(), MATERIAL_IRON, ')', 0, 0, CHAOTIC | NEUTRAL | LAWFUL, true);
	create("Orcrist", 2000, 70, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), MATERIAL_WOOD, ')', 0, 0, CHAOTIC, true);
	create("The Sceptre of Might", 2500, 30, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), MATERIAL_IRON, ')', PROPERTY_MAGICRES, 0, LAWFUL, true);
	create("Snickersnee", 1200, 40, Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_IRON, ')', 0, 0, LAWFUL, true);
	create("The Staff of Aesculapius", 5000, 40, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 8), MATERIAL_WOOD, ')', 0, 0, LAWFUL, true);
	create("Sting", 800, 10, Attack(AT_CLAW, AD_PHYS, 1, 5), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), MATERIAL_WOOD, ')', 0, 0, CHAOTIC, true);
	create("Stormbringer", 8000, 40, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(AT_CLAW, AD_PHYS, 1, 8), MATERIAL_IRON, ')', 0, 0, CHAOTIC, true);
	create("Sunsword", 1500, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), MATERIAL_IRON, ')', 0, 0, LAWFUL, true);
	create("Trollsbane", 200, 120, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_IRON, ')', 0, 0, CHAOTIC | NEUTRAL | LAWFUL, true);
	create("The Tsurugi of Muramasa", 4500, 60, Attack(AT_CLAW, AD_PHYS, 1, 16), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 2, 6), Attack(AT_CLAW, AD_PHYS, 1, 8), MATERIAL_METAL, ')', 0, 0, LAWFUL, false);
	create("Vorpal Blade", 4000, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), MATERIAL_IRON, ')', 0, 0, NEUTRAL, true);
	create("Werebane", 1500, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_SILVER, ')', 0, 0, CHAOTIC | NEUTRAL | LAWFUL, true);
}
