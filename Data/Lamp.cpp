#include "Lamp.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Lamp*> Lamp::_lamps;

/* protected constructors */
Lamp::Lamp(const string& name, int cost, int weight, int material, unsigned long long properties) : Tool(name, cost, weight, material, properties) {
}

/* destructor */
Lamp::~Lamp() {
}

/* public static methods */
void Lamp::init() {
	/* lamps */
	create("lamp", 10, 20, MATERIAL_COPPER, PROPERTY_RANDOM_APPEARANCE);
	create("oil lamp", 10, 20, MATERIAL_COPPER, 0);
	create("magic lamp", 50, 20, MATERIAL_COPPER, PROPERTY_MAGIC);
	create("brass lantern", 12, 30, MATERIAL_COPPER, 0);
}

const map<const string, const Lamp*>& Lamp::lamps() {
	return _lamps;
}

/* protected static methods */
void Lamp::addToMap(const string& name, const Lamp* lamp) {
	_lamps[name] = lamp;
	Tool::addToMap(name, lamp);
}

/* private static methods */
void Lamp::create(const string& name, int cost, int weight, int material, unsigned long long properties) {
	addToMap(name, new Lamp(name, cost, weight, material, properties));
}
