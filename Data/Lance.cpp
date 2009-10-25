#include "Lance.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Lance*> Lance::_lances;

/* protected constructors */
Lance::Lance(const string& name, const int& cost, const int& weight, const char& type, const int& material, const unsigned long long& properties, bool one_handed, const int& alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Lance::~Lance() {
}

/* public static methods */
void Lance::init() {
	/* lances */
	create("lance", 10, 180, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
}

const map<const string, const Lance*>& Lance::lances() {
	return _lances;
}

/* protected static methods */
void Lance::addToMap(const string& name, const Lance* lance) {
	_lances[name] = lance;
	Weapon::addToMap(name, lance);
}

/* private static methods */
void Lance::create(const string& name, const int& cost, const int& weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, const int& material, const char& type, const unsigned long long& properties, bool one_handed, const int& alignment) {
	addToMap(name, new Lance(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
