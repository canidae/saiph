#include "Armor.h"
#include "Boots.h"
#include "Cloak.h"
#include "Gloves.h"
#include "Helmet.h"
#include "Shield.h"
#include "Shirt.h"
#include "Suit.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Armor*> Armor::armors;

Armor::Armor(const string& name, int cost, int weight, int material, int slot, int ac, int mc, unsigned long long properties) : Item(name, cost, weight, ARMOR, material, properties), slot(slot), ac(ac), mc(mc) {
}

void Armor::addToMap(const string& name, Armor* armor) {
	Armor::armors[name] = armor;
	Item::addToMap(name, armor);
}

void Armor::init() {
	Boots::init();
	Cloak::init();
	Gloves::init();
	Helmet::init();
	Shield::init();
	Shirt::init();
	Suit::init();
	// TODO: fill out as more data is typed in
}
