#include "Boots.h"

using namespace data;
using namespace std;

/* initialize static variables */
map<string, Boots*> Boots::boots;

Boots::Boots(const string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : Armor(name, cost, weight, material, SLOT_BOOTS, ac, mc, properties) {
}

void Boots::addToMap(const string& name, Boots* boots) {
	Boots::boots[name] = boots;
	Armor::addToMap(name, boots);
}

void Boots::create(const string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
	addToMap(name, new Boots(name, cost, weight, material, ac, mc, properties));
}

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

	/* aliases */
	addToMap("walking shoes", boots["low boots"]);
	addToMap("jackboots", boots["high boots"]);
	addToMap("hard shoes", boots["iron shoes"]);

	/* TODO: random appearances: */
	// buckled, combat, hiking, jungle, riding, snow, mud
}
