#include "UnicornHorn.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, UnicornHorn*> UnicornHorn::unicorn_horns;

UnicornHorn::UnicornHorn(const string& name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

void UnicornHorn::addToMap(const string& name, UnicornHorn* unicorn_horn) {
	UnicornHorn::unicorn_horns[name] = unicorn_horn;
	Weapon::addToMap(name, unicorn_horn);
}

void UnicornHorn::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new UnicornHorn(name, cost, weight, item_class, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}

void UnicornHorn::init() {
	/* unicorn horns */
	create("unicorn horn", 100, 20, Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), MATERIAL_BONE, '(', 0, false, CHAOTIC | NEUTRAL | LAWFUL);
}
