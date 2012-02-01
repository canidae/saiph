#include "Data/Dart.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Dart*> Dart::_darts;

/* protected constructors */
Dart::Dart(const string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, category, material, properties, one_handed, alignment, -P_DART, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Dart::~Dart() {
}

/* public static methods */
void Dart::init() {
	/* darts */
	create("dart", 2, 1, Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 2), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("poisoned dart", _darts["dart"]);
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
void Dart::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Dart(name, cost, weight, category, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
