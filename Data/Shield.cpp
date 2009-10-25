#include "Shield.h"

#include "../Globals.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Shield*> Shield::_shields;

/* protected constructors */
Shield::Shield(const string& name, const int& cost, const int& weight, const int& material, const int& ac, const int& mc, const unsigned long long& properties) : Armor(name, cost, weight, material, SLOT_SHIELD, ac, mc, properties) {
}

/* destructor */
Shield::~Shield() {
}

/* public static methods */
void Shield::init() {
	create("large shield", 10, 100, MATERIAL_IRON, 2, 0, 0);
	create("Uruk-hai shield", 7, 50, MATERIAL_IRON, 1, 0, 0);
	create("elven shield", 7, 40, MATERIAL_WOOD, 2, 0, 0);
	create("small shield", 3, 30, MATERIAL_WOOD, 1, 0, 0);
	create("dwarvish roundshield", 10, 100, MATERIAL_IRON, 2, 0, 0);
	create("shield of reflection", 50, 50, MATERIAL_SILVER, 2, 0, PROPERTY_REFLECTION | PROPERTY_MAGIC);
	create("orcish shield", 7, 50, MATERIAL_IRON, 1, 0, 0);

	addToMap("blue and green shield", _shields["elven shield"]);
	addToMap("large round shield", _shields["dwarvish roundshield"]);
	addToMap("red-eyed shield", _shields["orcish shield"]);
	addToMap("polished silver shield", _shields["shield of reflection"]);
	addToMap("white-handed shield", _shields["Uruk-hai shield"]);
}

const map<const string, const Shield*>& Shield::shields() {
	return _shields;
}

/* protected static methods */
void Shield::addToMap(const string& name, const Shield* shield) {
	_shields[name] = shield;
	Armor::addToMap(name, shield);
}

/* private static methods */
void Shield::create(const string& name, const int& cost, const int& weight, const int& material, const int& ac, const int& mc, const unsigned long long& properties) {
	addToMap(name, new Shield(name, cost, weight, material, ac, mc, properties));
}