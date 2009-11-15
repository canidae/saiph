#include "Axe.h"
#include "Boomerang.h"
#include "Bow.h"
#include "Broadsword.h"
#include "Club.h"
#include "Crossbow.h"
#include "Dagger.h"
#include "Dart.h"
#include "Flail.h"
#include "Hammer.h"
#include "Javelin.h"
#include "Knife.h"
#include "Lance.h"
#include "Longsword.h"
#include "Mace.h"
#include "MorningStar.h"
#include "Pickaxe.h"
#include "Polearm.h"
#include "Quarterstaff.h"
#include "Saber.h"
#include "Scimitar.h"
#include "Shortsword.h"
#include "Shuriken.h"
#include "Sling.h"
#include "Spear.h"
#include "Trident.h"
#include "TwoHandedSword.h"
#include "UnicornHorn.h"
#include "Weapon.h"
#include "Whip.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Weapon*> Weapon::_weapons;

/* protected constructors */
Weapon::Weapon(const string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Item(name, cost, weight, category, material, properties), _one_handed(one_handed), _alignment(alignment) {
	_attack_small.push_back(as0);
	_attack_small.push_back(as1);
	_attack_small.push_back(as2);
	_attack_large.push_back(al0);
	_attack_large.push_back(al1);
	_attack_large.push_back(al2);
}

/* destructor */
Weapon::~Weapon() {
}

/* public static methods */
void Weapon::init() {
	Axe::init();
	Boomerang::init();
	Bow::init();
	Broadsword::init();
	Club::init();
	Crossbow::init();
	Dagger::init();
	Dart::init();
	Flail::init();
	Hammer::init();
	Javelin::init();
	Knife::init();
	Lance::init();
	Longsword::init();
	Mace::init();
	MorningStar::init();
	Pickaxe::init();
	Polearm::init();
	Quarterstaff::init();
	Saber::init();
	Scimitar::init();
	Shortsword::init();
	Shuriken::init();
	Sling::init();
	Spear::init();
	Trident::init();
	TwoHandedSword::init();
	UnicornHorn::init();
	Whip::init();
}

const map<const string, const Weapon*> Weapon::weapons() {
	return _weapons;
}

/* public methods */
bool Weapon::oneHanded() {
	return _one_handed;
}

int Weapon::alignment() {
	return _alignment;
}

const vector<Attack>& Weapon::attackSmall() {
	return _attack_small;
}

const vector<Attack>& Weapon::attackLarge() {
	return _attack_large;
}

/* protected static methods */
void Weapon::addToMap(const string& name, const Weapon* weapon) {
	_weapons[name] = weapon;
	Item::addToMap(name, weapon);
}
