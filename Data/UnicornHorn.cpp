#include "UnicornHorn.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const UnicornHorn*> UnicornHorn::_unicorn_horns;

/* protected constructors */
UnicornHorn::UnicornHorn(const string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, category, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
UnicornHorn::~UnicornHorn() {
}

/* public static methods */
void UnicornHorn::init() {
	/* unicorn horns */
	create("unicorn horn", 100, 20, Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), MATERIAL_BONE, TOOL, PROPERTY_MAGIC, false, CHAOTIC | NEUTRAL | LAWFUL);
}

const map<const string, const UnicornHorn*>& UnicornHorn::unicornHorns() {
	return _unicorn_horns;
}

/* protected static methods */
void UnicornHorn::addToMap(const string& name, const UnicornHorn* unicorn_horn) {
	_unicorn_horns[name] = unicorn_horn;
	Weapon::addToMap(name, unicorn_horn);
}

/* private static methods */
void UnicornHorn::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new UnicornHorn(name, cost, weight, category, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
