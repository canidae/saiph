#include "TwoHandedSword.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const TwoHandedSword*> TwoHandedSword::_two_handed_swords;

/* protected constructor */
TwoHandedSword::TwoHandedSword(const string& name, int cost, int weight, char type, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
TwoHandedSword::~TwoHandedSword() {
}

/* public static methods */
void TwoHandedSword::init() {
	/* two-handed swords */
	create("two-handed sword", 50, 150, Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 3, 6), Attack(), Attack(), MATERIAL_IRON, ')', 0, false, CHAOTIC | NEUTRAL | LAWFUL);
	create("tsurugi", 500, 60, Attack(AT_CLAW, AD_PHYS, 1, 16), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 2, 6), Attack(), MATERIAL_METAL, ')', 0, false, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("long samurai sword", _two_handed_swords["tsurugi"]);

	/* artifact two-handed swords */
	create("The Tsurugi of Muramasa", 4500, 60, Attack(AT_CLAW, AD_PHYS, 1, 16), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 2, 6), Attack(AT_CLAW, AD_PHYS, 1, 8), MATERIAL_METAL, ')', PROPERTY_MAGIC | PROPERTY_ARTIFACT, false, LAWFUL);
}

const map<const string, const TwoHandedSword*>& TwoHandedSword::twoHandedSwords() {
	return _two_handed_swords;
}

/* protected static methods */
void TwoHandedSword::addToMap(const string& name, const TwoHandedSword* two_handed_sword) {
	_two_handed_swords[name] = two_handed_sword;
	Weapon::addToMap(name, two_handed_sword);
}

/* private static methods */
void TwoHandedSword::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char type, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new TwoHandedSword(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
