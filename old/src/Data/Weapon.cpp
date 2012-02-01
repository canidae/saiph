#include "Data/Weapon.h"

#include "Data/Axe.h"
#include "Data/Boomerang.h"
#include "Data/Bow.h"
#include "Data/BowAmmo.h"
#include "Data/Broadsword.h"
#include "Data/Club.h"
#include "Data/Crossbow.h"
#include "Data/CrossbowAmmo.h"
#include "Data/Dagger.h"
#include "Data/Dart.h"
#include "Data/Flail.h"
#include "Data/Hammer.h"
#include "Data/Javelin.h"
#include "Data/Knife.h"
#include "Data/Lance.h"
#include "Data/Longsword.h"
#include "Data/Mace.h"
#include "Data/MorningStar.h"
#include "Data/Pickaxe.h"
#include "Data/Polearm.h"
#include "Data/Quarterstaff.h"
#include "Data/Saber.h"
#include "Data/Scimitar.h"
#include "Data/Shortsword.h"
#include "Data/Shuriken.h"
#include "Data/Sling.h"
#include "Data/SlingAmmo.h"
#include "Data/Spear.h"
#include "Data/Trident.h"
#include "Data/TwoHandedSword.h"
#include "Data/UnicornHorn.h"
#include "Data/Whip.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Weapon*> Weapon::_weapons;

/* protected constructors */
Weapon::Weapon(const string& name, int cost, int weight, char category, int material, unsigned long long properties, bool one_handed, int alignment, int type, const Attack& as0, const Attack& as1, const Attack& as2, const Attack& al0, const Attack& al1, const Attack& al2) : Item(name, cost, weight, category, material, properties), _one_handed(one_handed), _alignment(alignment), _type(type) {
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
	BowAmmo::init();
	Broadsword::init();
	Club::init();
	Crossbow::init();
	CrossbowAmmo::init();
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
	SlingAmmo::init();
	Spear::init();
	Trident::init();
	TwoHandedSword::init();
	UnicornHorn::init();
	Whip::init();
}

const map<const string, const Weapon*>& Weapon::weapons() {
	return _weapons;
}

/* public methods */
bool Weapon::oneHanded() const {
	return _one_handed;
}

int Weapon::alignment() const {
	return _alignment;
}

int Weapon::type() const {
	return _type;
}

const vector<Attack>& Weapon::attackSmall() const {
	return _attack_small;
}

const vector<Attack>& Weapon::attackLarge() const {
	return _attack_large;
}

/* protected static methods */
void Weapon::addToMap(const string& name, const Weapon* weapon) {
	_weapons[name] = weapon;
	Item::addToMap(name, weapon);
}
