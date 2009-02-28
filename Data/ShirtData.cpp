#include "ShirtData.h"

using namespace std;

/* initialize static variables */
map<string, ShirtData*> ShirtData::shirts;

ShirtData::ShirtData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : ArmorData(name, cost, weight, material, ARMOR_SHIRT, ac, mc, properties) {
}

void create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
	ShirtData* foo = new ShirtData(name, cost, weight, material, ac, mc, properties);
	ShirtData::shirts[foo->name] = foo;
	ArmorData::armors[foo->name] = foo;
	ItemData::items[foo->name] = foo;
}

void ShirtData::init() {
	create("Hawaiian shirt", 3, 5, MATERIAL_CLOTH, 0, 0, 0);
	create("T-shirt", 2, 5, MATERIAL_CLOTH, 0, 0, 0);
}
