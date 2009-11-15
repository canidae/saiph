#include "Pickaxe.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Pickaxe*> Pickaxe::_pickaxes;

/* protected constructors */
Pickaxe::Pickaxe(const string& name, int cost, int weight, char type, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Pickaxe::~Pickaxe() {
}

/* public static methods */
void Pickaxe::init() {
	/* pick axes */
	create("pick-axe", 50, 100, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), MATERIAL_IRON, TOOL, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("dwarvish mattock", 50, 120, Attack(AT_CLAW, AD_PHYS, 1, 12), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(AT_CLAW, AD_PHYS, 2, 6), Attack(), MATERIAL_IRON, WEAPON, 0, false, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("broad pick", _pickaxes["dwarvish mattock"]);
}

const map<const string, const Pickaxe*>& Pickaxe::pickaxes() {
	return _pickaxes;
}

/* protected static methods */
void Pickaxe::addToMap(const string& name, const Pickaxe* pick_axe) {
	_pickaxes[name] = pick_axe;
	Weapon::addToMap(name, pick_axe);
}

/* private static methods */
void Pickaxe::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char type, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Pickaxe(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
