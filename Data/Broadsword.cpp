#include "Broadsword.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Broadsword*> Broadsword::_broadswords;

/* protected constructors */
Broadsword::Broadsword(const string& name, int cost, int weight, char type, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Broadsword::~Broadsword() {
}

/* public static methods */
void Broadsword::init() {
	/* broad swords */
	create("broadsword", 10, 70, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("runesword", 300, 40, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("elven broadsword", 10, 70, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), MATERIAL_WOOD, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("runed broadsword", _broadswords["runesword"]);

	/* artifact broad swords */
	create("Demonbane", 2500, 40, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), MATERIAL_IRON, ')', true, PROPERTY_MAGIC | PROPERTY_ARTIFACT, LAWFUL);
	create("Orcrist", 2000, 70, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 4), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), MATERIAL_WOOD, ')', true, PROPERTY_MAGIC | PROPERTY_ARTIFACT, CHAOTIC);
	create("Stormbringer", 8000, 40, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(AT_CLAW, AD_PHYS, 1, 8), MATERIAL_IRON, ')', true, PROPERTY_MAGIC | PROPERTY_ARTIFACT, CHAOTIC);
}

const map<const string, const Broadsword*>& Broadsword::broadswords() {
	return _broadswords;
}

/* protected static methods */
void Broadsword::addToMap(const string& name, const Broadsword* broadsword) {
	_broadswords[name] = broadsword;
	Weapon::addToMap(name, broadsword);
}

/* private static methods */
void Broadsword::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char type, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Broadsword(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
