#include "Data/Armor.h"

#include "Globals.h"
#include "Data/Boots.h"
#include "Data/Cloak.h"
#include "Data/Gloves.h"
#include "Data/Helmet.h"
#include "Data/Shield.h"
#include "Data/Shirt.h"
#include "Data/Suit.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Armor*> Armor::_armors;

/* protected constructors */
Armor::Armor(const string& name, int cost, int weight, int material, int slot, int ac, int mc, unsigned long long properties) : Item(name, cost, weight, ARMOR, material, properties), _slot(slot), _ac(ac), _mc(mc) {
}

/* destructor */
Armor::~Armor() {
}

/* public static methods */
void Armor::init() {
	Boots::init();
	Cloak::init();
	Gloves::init();
	Helmet::init();
	Shield::init();
	Shirt::init();
	Suit::init();
}

const map<const string, const Armor*>& Armor::armors() {
	return _armors;
}

/* public methods */
int Armor::slot() const {
	return _slot;
}

int Armor::ac() const {
	return _ac;
}

int Armor::mc() const {
	return _mc;
}

/* protected static methods */
void Armor::addToMap(const string& name, const Armor* armor) {
	_armors[name] = armor;
	Item::addToMap(name, armor);
}

