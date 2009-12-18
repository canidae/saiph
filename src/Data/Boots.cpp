#include "Data/Boots.h"

#include "Globals.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<const string, const Boots*> Boots::_boots;

/* protected constructors */
Boots::Boots(const string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : Armor(name, cost, weight, material, SLOT_BOOTS, ac, mc, properties) {
}

/* destructor */
Boots::~Boots() {
}

/* public static methods */
void Boots::init() {
	/* boots */
	create("low boots", 8, 10, MATERIAL_LEATHER, 1, 0, 0);
	create("high boots", 12, 20, MATERIAL_LEATHER, 2, 0, 0);
	create("iron shoes", 16, 50, MATERIAL_IRON, 2, 0, 0);
	create("elven boots", 8, 15, MATERIAL_LEATHER, 1, 0, PROPERTY_STEALTH | PROPERTY_MAGIC);
	create("kicking boots", 8, 15, MATERIAL_IRON, 1, 0, PROPERTY_KICKING | PROPERTY_MAGIC);
	create("fumble boots", 30, 20, MATERIAL_LEATHER, 1, 0, PROPERTY_FUMBLING | PROPERTY_MAGIC);
	create("levitation boots", 30, 15, MATERIAL_LEATHER, 1, 0, PROPERTY_LEVITATION | PROPERTY_MAGIC);
	create("jumping boots", 50, 20, MATERIAL_LEATHER, 1, 0, PROPERTY_JUMPING | PROPERTY_MAGIC);
	create("speed boots", 50, 20, MATERIAL_LEATHER, 1, 0, PROPERTY_VERYFAST | PROPERTY_MAGIC);
	create("water walking boots", 50, 20, MATERIAL_LEATHER, 1, 0, PROPERTY_WATERWALKING | PROPERTY_MAGIC);

	/* random appearances */
	create("mud boots", 50, 20, MATERIAL_IRON, 1, 0, PROPERTY_RANDOM_APPEARANCE);
	create("snow boots", 50, 20, MATERIAL_IRON, 1, 0, PROPERTY_RANDOM_APPEARANCE);
	create("riding boots", 50, 20, MATERIAL_IRON, 1, 0, PROPERTY_RANDOM_APPEARANCE);
	create("buckled boots", 50, 20, MATERIAL_IRON, 1, 0, PROPERTY_RANDOM_APPEARANCE);
	create("hiking boots", 50, 20, MATERIAL_IRON, 1, 0, PROPERTY_RANDOM_APPEARANCE);
	create("combat boots", 50, 20, MATERIAL_IRON, 1, 0, PROPERTY_RANDOM_APPEARANCE);
	create("jungle boots", 50, 20, MATERIAL_IRON, 1, 0, PROPERTY_RANDOM_APPEARANCE);

	/* aliases */
	addToMap("walking shoes", _boots["low boots"]);
	addToMap("jackboots", _boots["high boots"]);
	addToMap("hard shoes", _boots["iron shoes"]);
}

const map<const string, const Boots*>& Boots::boots() {
	return _boots;
}

/* protected static methods */
void Boots::addToMap(const string& name, const Boots* boots) {
	_boots[name] = boots;
	Armor::addToMap(name, boots);
}

/* private static methods */
void Boots::create(const string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
	addToMap(name, new Boots(name, cost, weight, material, ac, mc, properties));
}
