#include "BootsData.h"

using namespace std;

/* initialize static variables */
map<string, BootsData *> BootsData::boots;

BootsData::BootsData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : ArmorData(name, cost, weight, material, ARMOR_BOOTS, ac, mc, properties) {
}

void BootsData::addToMap(const std::string &name, BootsData *boots) {
	BootsData::boots[name] = boots;
	ArmorData::addToMap(name, boots);
}

void BootsData::create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
	addToMap(name, new BootsData(name, cost, weight, material, ac, mc, properties));
}

void BootsData::init() {
	//     name                 cost  weight  material          ac mc properties
	create("low boots",            8,     10, MATERIAL_LEATHER, 1, 0, 0);
	create("high boots",          12,     20, MATERIAL_LEATHER, 2, 0, 0);
	create("iron shoes",          16,     50, MATERIAL_IRON,    2, 0, 0);
	create("elven boots",          8,     15, MATERIAL_LEATHER, 1, 0, PROPERTY_STEALTH | PROPERTY_MAGIC);
	create("kicking boots",        8,     15, MATERIAL_IRON,    1, 0, PROPERTY_KICKING | PROPERTY_MAGIC);
	create("fumble boots",        30,     20, MATERIAL_LEATHER, 1, 0, PROPERTY_FUMBLING | PROPERTY_MAGIC);
	create("levitation boots",    30,     15, MATERIAL_LEATHER, 1, 0, PROPERTY_LEVITATION | PROPERTY_MAGIC);
	create("jumping boots",       50,     20, MATERIAL_LEATHER, 1, 0, PROPERTY_JUMPING | PROPERTY_MAGIC);
	create("speed boots",         50,     20, MATERIAL_LEATHER, 1, 0, PROPERTY_VERYFAST | PROPERTY_MAGIC);
	create("water walking boots", 50,     20, MATERIAL_LEATHER, 1, 0, PROPERTY_WATERWALKING | PROPERTY_MAGIC);

	// these appearances are constant
	addToMap("walking shoes", boots["low boots"]);
	addToMap("jackboots", boots["high boots"]);
	addToMap("hard shoes", boots["iron shoes"]);

	// TODO: random appearances:
	// buckled, combat, hiking, jungle, riding, snow, mud
}
