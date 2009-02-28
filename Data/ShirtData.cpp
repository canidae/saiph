#include "ShirtData.h"

using namespace std;

/* initialize static variables */
map<string, ShirtData *> ShirtData::shirts;

ShirtData::ShirtData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : ArmorData(name, cost, weight, material, ARMOR_SHIRT, ac, mc, properties) {
}

void ShirtData::addToMap(const std::string &name, ShirtData *shirt) {
	ShirtData::shirts[name] = shirt;
	ArmorData::addToMap(name, shirt);
}

void ShirtData::create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
	addToMap(name, new ShirtData(name, cost, weight, material, ac, mc, properties));
}

void ShirtData::init() {
	//     name           cost  weight  material        ac mc properties
	create("Hawaiian shirt", 3,      5, MATERIAL_CLOTH, 0, 0, 0);
	create("T-shirt",        2,      5, MATERIAL_CLOTH, 0, 0, 0);
}
