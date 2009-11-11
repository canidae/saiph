#include "Amulet.h"

#include "../Globals.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Amulet*> Amulet::_amulets;

/* protected constructors */
Amulet::Amulet(const string& name, int cost, int weight, int material, unsigned long long properties, bool confer_effect) : Item(name, cost, weight, AMULET, material, properties), _confer_effect(confer_effect) {
}

/* destructor */
Amulet::~Amulet() {
}

/* public static methods */
void Amulet::init() {
	create("amulet of change", 150, 20, MATERIAL_METAL, PROPERTY_SEX_CHANGE, true);
	create("amulet of ESP", 150, 20, MATERIAL_METAL, PROPERTY_ESP, true);
	create("amulet of life saving", 150, 20, MATERIAL_METAL, PROPERTY_LIFE_SAVING, false);
	create("amulet of magical breathing", 150, 20, MATERIAL_METAL, PROPERTY_MAGICAL_BREATHING, true);
	create("amulet of reflection", 150, 20, MATERIAL_METAL, PROPERTY_REFLECTION, false);
	create("amulet of restful sleep", 150, 20, MATERIAL_METAL, PROPERTY_RESTFUL_SLEEP, true);
	create("amulet of strangulation", 150, 20, MATERIAL_METAL, PROPERTY_STRANGULATION, true);
	create("amulet of unchanging", 150, 20, MATERIAL_METAL, PROPERTY_UNCHANGING, false);
	create("amulet versus poison", 150, 20, MATERIAL_METAL, PROPERTY_POISON, true);
	create("cheap plastic imitation of the Amulet of Yendor", 150, 20, MATERIAL_PLASTIC, 0, false);
	create("Amulet of Yendor", 150, 20, MATERIAL_METAL, 0, false);

	/* random appearance amulets */
	create("circular amulet", 150, 20, MATERIAL_METAL, PROPERTY_RANDOM_APPEARANCE, false);
	create("spherical amulet", 150, 20, MATERIAL_METAL, PROPERTY_RANDOM_APPEARANCE, false);
	create("oval amulet", 150, 20, MATERIAL_METAL, PROPERTY_RANDOM_APPEARANCE, false);
	create("triangular amulet", 150, 20, MATERIAL_METAL, PROPERTY_RANDOM_APPEARANCE, false);
	create("pyramidal amulet", 150, 20, MATERIAL_METAL, PROPERTY_RANDOM_APPEARANCE, false);
	create("square amulet", 150, 20, MATERIAL_METAL, PROPERTY_RANDOM_APPEARANCE, false);
	create("concave amulet", 150, 20, MATERIAL_METAL, PROPERTY_RANDOM_APPEARANCE, false);
	create("hexagonal amulet", 150, 20, MATERIAL_METAL, PROPERTY_RANDOM_APPEARANCE, false);
	create("octagonal amulet", 150, 20, MATERIAL_METAL, PROPERTY_RANDOM_APPEARANCE, false);
}

const map<const string, const Amulet*>& Amulet::amulets() {
	return _amulets;
}

/* public methods */
bool Amulet::conferEffect() const {
	return _confer_effect;
}

/* protected static methods */
void Amulet::addToMap(const string& name, const Amulet* amulet) {
	_amulets[name] = amulet;
	Item::addToMap(name, amulet);
}

/* private static methods */
void Amulet::create(const string& name, int cost, int weight, int material, unsigned long long properties, bool confer_effect) {
	addToMap(name, new Amulet(name, cost, weight, material, properties, confer_effect));
}
