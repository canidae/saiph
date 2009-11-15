#include "Longsword.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Longsword*> Longsword::_longswords;

/* protected constructors */
Longsword::Longsword(const string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, category, material, properties, one_handed, alignment, WEAPON_LONGSWORD, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Longsword::~Longsword() {
}

/* public static methods */
void Longsword::init() {
	/* long swords */
	create("long sword", 15, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("katana", 80, 40, Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("samurai sword", _longswords["katana"]);

	/* artifact long swords */
	create("Demonbane", 2500, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, LAWFUL);
	create("Excalibur", 4000, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), MATERIAL_IRON, WEAPON, PROPERTY_MAGIC | PROPERTY_SEARCHING | PROPERTY_ARTIFACT, true, LAWFUL);
	create("Fire Brand", 3000, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_FIRE, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(AT_CLAW, AD_FIRE, 1, 12), Attack(), MATERIAL_IRON, WEAPON, PROPERTY_MAGIC | PROPERTY_FIRE | PROPERTY_ARTIFACT, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("Frost Brand", 3000, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_COLD, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(AT_CLAW, AD_COLD, 1, 12), Attack(), MATERIAL_IRON, WEAPON, PROPERTY_MAGIC | PROPERTY_COLD | PROPERTY_ARTIFACT, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("Giantslayer", 200, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), MATERIAL_IRON, WEAPON, PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("Snickersnee", 1200, 40, Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), MATERIAL_IRON, WEAPON, PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, LAWFUL);
	create("Sunsword", 1500, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), MATERIAL_IRON, WEAPON, PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, LAWFUL);
	create("Vorpal Blade", 4000, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), MATERIAL_IRON, WEAPON, PROPERTY_MAGIC | PROPERTY_ARTIFACT, true, NEUTRAL);
}

const map<const string, const Longsword*>& Longsword::longswords() {
	return _longswords;
}

/* protected static methods */
void Longsword::addToMap(const string& name, const Longsword* long_sword) {
	_longswords[name] = long_sword;
	Weapon::addToMap(name, long_sword);
}

/* private static methods */
void Longsword::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Longsword(name, cost, weight, category, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
