#include "Dart.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Dart*> Dart::_darts;

/* protected constructors */
Dart::Dart(const string& name, const int& cost, const int& weight, const char& type, const int& material, const unsigned long long& properties, bool one_handed, const int& alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Dart::~Dart() {
}

/* public static methods */
void Dart::init() {
	/* darts */
	create("dart", 2, 1, Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_IRON, ')', 0, true, CHAOTIC | NEUTRAL | LAWFUL);
}

const map<const string, const Dart*>& Dart::darts() {
	return _darts;
}

/* protected static methods */
void Dart::addToMap(const string& name, const Dart* dart) {
	_darts[name] = dart;
	Weapon::addToMap(name, dart);
}

/* private static methods */
void Dart::create(const string& name, const int& cost, const int& weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, const int& material, const char& type, const unsigned long long& properties, bool one_handed, const int& alignment) {
	addToMap(name, new Dart(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
