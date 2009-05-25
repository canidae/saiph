#include "Axe.h"
#include "Boomerang.h"
#include "Bow.h"
#include "BroadSword.h"
#include "Club.h"
#include "Crossbow.h"
#include "Dagger.h"
#include "Dart.h"
#include "Flail.h"
#include "Hammer.h"
#include "Javelin.h"
#include "Knife.h"
#include "Lance.h"
#include "LongSword.h"
#include "Mace.h"
#include "MorningStar.h"
#include "PickAxe.h"
#include "Polearm.h"
#include "Quarterstaff.h"
#include "Saber.h"
#include "Scimitar.h"
#include "ShortSword.h"
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
map<string, Weapon *> Weapon::weapons;

Weapon::Weapon(const string &name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2) : Item(name, cost, weight, item_class, material, properties), one_handed(one_handed), alignment(alignment) {
	attack_small[0] = as0;
	attack_small[1] = as1;
	attack_small[2] = as2;
	attack_large[0] = al0;
	attack_large[1] = al1;
	attack_large[2] = al2;
}

void Weapon::addToMap(const string &name, Weapon *weapon) {
	Weapon::weapons[name] = weapon;
	Item::addToMap(name, weapon);
}

void Weapon::init() {
	Axe::init();
	Boomerang::init();
	Bow::init();
	BroadSword::init();
	Club::init();
	Crossbow::init();
	Dagger::init();
	Dart::init();
	Flail::init();
	Hammer::init();
	Javelin::init();
	Knife::init();
	Lance::init();
	LongSword::init();
	Mace::init();
	MorningStar::init();
	PickAxe::init();
	Polearm::init();
	Quarterstaff::init();
	Saber::init();
	Scimitar::init();
	ShortSword::init();
	Shuriken::init();
	Sling::init();
	Spear::init();
	Trident::init();
	TwoHandedSword::init();
	UnicornHorn::init();
	Whip::init();
}
