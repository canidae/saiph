#include "Trident.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Trident*> Trident::_tridents;

/* protected static methods */
Trident::Trident(const string& name, const int& cost, const int& weight, const char& type, const int& material, const unsigned long long& properties, bool one_handed, const int& alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Trident::~Trident() {
}

/* public static methods */
void Trident::init() {
	/* tridents */
	create("trident", 5, 25, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), Attack(AT_CLAW, AD_PHYS, 3, 4), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
}

const map<const string, const Trident*>& Trident::tridents() {
	return _tridents;
}

/* protected static methods */
void Trident::addToMap(const string& name, const Trident* trident) {
	_tridents[name] = trident;
	Weapon::addToMap(name, trident);
}

/* private static methods */
void Trident::create(const string& name, const int& cost, const int& weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, const int& material, const char& type, const unsigned long long& properties, bool one_handed, const int& alignment) {
	addToMap(name, new Trident(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}