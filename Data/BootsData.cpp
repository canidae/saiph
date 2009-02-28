#include "BootsData.h"

using namespace std;

/* initialize static variables */
map<string, BootsData*> BootsData::boots;

BootsData::BootsData(const std::string &name = "", int cost = 0, int weight = 0, int material = 0, int ac = 0, int mc = 0, unsigned long long properties = 0) : ArmorData(name, cost, weight, material, ARMOR_SUIT, ac, mc, properties) {
}

namespace { //an anonymous namespace, to avoid linker errors from the various create()s
	inline void stuffInMaps(const std::string& name, BootsData* foo) {
		BootsData::boots[name] = foo;
		ArmorData::armors[name] = foo;
		ItemData::items[name] = foo;
	}
	
	inline void create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
		BootsData* foo = new BootsData(name, cost, weight, material, ac, mc, properties);
		stuffInMaps(foo->name, foo);
	}
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

	//these appearances are constant
	stuffInMaps("walking shoes", boots["low boots"]);
	stuffInMaps("jackboots", boots["high boots"]);
	stuffInMaps("hard shoes", boots["iron shoes"]);

	//TODO: random appearances:
	//buckled, combat, hiking, jungle, riding, snow, mud
}
