#include "ShirtData.h"

using namespace std;

/* initialize static variables */
map<string, ShirtData*> ShirtData::shirts;

ShirtData::ShirtData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : ArmorData(name, cost, weight, material, ARMOR_SHIRT, ac, mc, properties) {
	shirts[name] = this;
	ArmorData::armors[name] = this;
	ItemData::items[name] = this;
}

void ShirtData::init() {
	ShirtData("Hawaiian shirt", 3, 5, MATERIAL_CLOTH, 0, 0, 0);
	ShirtData("T-shirt", 2, 5, MATERIAL_CLOTH, 0, 0, 0);
}
