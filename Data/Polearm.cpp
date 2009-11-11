#include "Polearm.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Polearm*> Polearm::_polearms;

/* protected constructors */
Polearm::Polearm(const string& name, int cost, int weight, char type, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Polearm::~Polearm() {
}

/* public static methods */
void Polearm::init() {
	/* polearms */
	create("partisan", 10, 80, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), MATERIAL_IRON, ')', 0, false, CHAOTIC | NEUTRAL | LAWFUL);
	create("fauchard", 5, 60, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_IRON, ')', 0, false, CHAOTIC | NEUTRAL | LAWFUL);
	create("glaive", 6, 75, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), Attack(), MATERIAL_IRON, ')', 0, false, CHAOTIC | NEUTRAL | LAWFUL);
	create("bec-de-corbin", 8, 100, Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_IRON, ')', 0, false, CHAOTIC | NEUTRAL | LAWFUL);
	create("spetum", 5, 50, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(AT_CLAW, AD_PHYS, 1, 1), Attack(), Attack(AT_CLAW, AD_PHYS, 2, 6), Attack(), Attack(), MATERIAL_IRON, ')', 0, false, CHAOTIC | NEUTRAL | LAWFUL);
	create("lucern hammer", 7, 150, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), MATERIAL_IRON, ')', 0, false, CHAOTIC | NEUTRAL | LAWFUL);
	create("guisarme", 5, 80, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 8), Attack(), Attack(), MATERIAL_IRON, ')', 0, false, CHAOTIC | NEUTRAL | LAWFUL);
	create("ranseur", 6, 50, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), MATERIAL_IRON, ')', 0, false, CHAOTIC | NEUTRAL | LAWFUL);
	create("voulge", 5, 125, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), MATERIAL_IRON, ')', 0, false, CHAOTIC | NEUTRAL | LAWFUL);
	create("bill-guisarme", 7, 120, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), Attack(), MATERIAL_IRON, ')', 0, false, CHAOTIC | NEUTRAL | LAWFUL);
	create("bardiche", 7, 120, Attack(AT_CLAW, AD_PHYS, 2, 4), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 3, 4), Attack(), Attack(), MATERIAL_IRON, ')', 0, false, CHAOTIC | NEUTRAL | LAWFUL);
	create("halberd", 10, 150, Attack(AT_CLAW, AD_PHYS, 1, 10), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 2, 6), Attack(), Attack(), MATERIAL_IRON, ')', 0, false, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("vulgar polearm", _polearms["partisan"]);
	addToMap("pole sickle", _polearms["fauchard"]);
	addToMap("single-edged polearm", _polearms["glaive"]);
	addToMap("beaked polearm", _polearms["bec-de-corbin"]);
	addToMap("forked polearm", _polearms["spetum"]);
	addToMap("pronged polearm", _polearms["lucern hammer"]);
	addToMap("pruning hook", _polearms["guisarme"]);
	addToMap("hilted polearm", _polearms["ranseur"]);
	addToMap("pole cleaver", _polearms["voulge"]);
	addToMap("hooked polearm", _polearms["bill-guisarme"]);
	addToMap("long poleaxe", _polearms["bardiche"]);
	addToMap("angled poleaxe", _polearms["halberd"]);
}

const map<const string, const Polearm*>& Polearm::polearms() {
	return _polearms;
}

/* protected static methods */
void Polearm::addToMap(const string& name, const Polearm* polearm) {
	_polearms[name] = polearm;
	Weapon::addToMap(name, polearm);
}

/* private static methods */
void Polearm::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char type, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Polearm(name, cost, weight, type, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
