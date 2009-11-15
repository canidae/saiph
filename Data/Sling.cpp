#include "Sling.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Sling*> Sling::_slings;

/* protected constructors */
Sling::Sling(const string& name, int cost, int weight, char type, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Sling::~Sling() {
}

/* public static methods */
void Sling::init() {
	/* slings */
	create("sling", 20, 3, Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_LEATHER, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("flintstone", 1, 10, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_MINERAL, GEM, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
}

const map<const string, const Sling*>& Sling::slings() {
	return _slings;
}

/* protected static methods */
void Sling::addToMap(const string& name, const Sling* sling) {
	_slings[name] = sling;
	Weapon::addToMap(name, sling);
}

/* private static methods */
void Sling::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char type, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Sling(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
