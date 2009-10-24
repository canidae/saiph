#include "Armor.h"

#include "../Globals.h"
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
map<const string, const Armor*> Armor::_armors;

/* constructors/destructor */
Armor::Armor(const string& name, const int& cost, const int& weight, const int& material, const int& slot, const int& ac, const int& mc, const unsigned long long& properties) : Item(name, cost, weight, ARMOR, material, properties), _slot(slot), _ac(ac), _mc(mc) {
}

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

/* public methods */
const int& Armor::slot() const {
	return _slot;
}

const int& Armor::ac() const {
	return _ac;
}

const int& Armor::mc() const {
	return _mc;
}

/* protected static methods */
void Armor::addToMap(const string& name, const Armor* armor) {
	_armors[name] = armor;
	Item::addToMap(name, armor);
}

