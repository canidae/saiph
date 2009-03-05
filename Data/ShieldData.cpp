#include "ShieldData.h"

using namespace std;

/* initialize static variables */
map<string, ShieldData *> ShieldData::shields;

ShieldData::ShieldData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : ArmorData(name, cost, weight, material, ARMOR_SUIT, ac, mc, properties) {
}

void ShieldData::addToMap(const std::string &name, ShieldData *shield) {
	ShieldData::shields[name] = shield;
	ArmorData::addToMap(name, shield);
}

void ShieldData::create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
	addToMap(name, new ShieldData(name, cost, weight, material, ac, mc, properties));
}

void ShieldData::init() {
	create("large shield", 10, 100, MATERIAL_IRON, 2, 0, 0);
	create("Uruk-hai shield", 7, 50, MATERIAL_IRON, 1, 0, 0);
	create("elven shield", 7, 40, MATERIAL_WOOD, 2, 0, 0);
	create("small shield", 3, 30, MATERIAL_WOOD, 1, 0, 0);
	create("dwarvish roundshield", 10, 100, MATERIAL_IRON, 2, 0, 0);
	create("shield of reflection", 50, 50, MATERIAL_SILVER, 2, 0, PROPERTY_REFLECTION | PROPERTY_MAGIC);
	create("orcish shield", 7, 50, MATERIAL_IRON, 1, 0, 0);

	addToMap("blue and green shield", shields["elven shield"]);
	addToMap("large round shield", shields["dwarvish roundshield"]);
	addToMap("red-eyed shield", shields["orcish shield"]);
	addToMap("polished silver shield", shields["shield of reflection"]);
	addToMap("white-handed shield", shields["Uruk-hai shield"]);
}
