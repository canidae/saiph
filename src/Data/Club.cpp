#include "Data/Club.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Club*> Club::_clubs;

/* protected constructors */
Club::Club(const string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Weapon(name, cost, weight, category, material, properties, one_handed, alignment, WEAPON_CLUB, as0, as1, as2, al0, al1, al2) {
}

/* destructor */
Club::~Club() {
}

/* public static methods */
void Club::init() {
	/* clubs */
	create("club", 3, 30, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), MATERIAL_WOOD, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);
	create("aklys", 4, 15, Attack(AT_CLAW, AD_PHYS, 1, 6), Attack(), Attack(), Attack(AT_CLAW, AD_PHYS, 1, 3), Attack(), Attack(), MATERIAL_IRON, WEAPON, 0, true, CHAOTIC | NEUTRAL | LAWFUL);

	/* aliases */
	addToMap("thonged club", _clubs["aklys"]);
}

const map<const string, const Club*>& Club::clubs() {
	return _clubs;
}

/* protected static methods */
void Club::addToMap(const string& name, const Club* club) {
	_clubs[name] = club;
	Weapon::addToMap(name, club);
}

/* private static methods */
void Club::create(const string& name, int cost, int weight, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2, int material, char category, unsigned long long properties, bool one_handed, int alignment) {
	addToMap(name, new Club(name, cost, weight, category, material, properties, one_handed, alignment, as0, as1, as2, al0, al1, al2));
}
