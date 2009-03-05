#include "HelmetData.h"

using namespace std;

/* initialize static variables */
map<string, HelmetData *> HelmetData::helmets;

HelmetData::HelmetData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : ArmorData(name, cost, weight, material, ARMOR_SUIT, ac, mc, properties) {
}

void HelmetData::addToMap(const std::string &name, HelmetData *helmet) {
	HelmetData::helmets[name] = helmet;
	ArmorData::addToMap(name, helmet);
}

void HelmetData::create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
	addToMap(name, new HelmetData(name, cost, weight, material, ac, mc, properties));
}

void HelmetData::init() {
	create("helm of brilliance", 50, 50, MATERIAL_IRON, 1, 0, PROPERTY_MAGIC | PROPERTY_BRILLIANCE);
	create("helm of opposite alignment", 50, 50, MATERIAL_IRON, 1, 0, PROPERTY_MAGIC);
	create("dunce cap", 1, 4, MATERIAL_CLOTH, 0, 0, PROPERTY_MAGIC | PROPERTY_STUPIDITY);
	create("cornuthaum", 80, 4, MATERIAL_CLOTH, 0, 2, PROPERTY_MAGIC);
	create("helm of telepathy", 50, 50, MATERIAL_IRON, 1, 0, PROPERTY_MAGIC | PROPERTY_ESP);
	create("dwarvish iron helm", 20, 40, MATERIAL_IRON, 2, 0, 0);
	create("Mitre of Holiness", 2000, 50, MATERIAL_IRON, 1, 0, PROPERTY_MAGIC | PROPERTY_BRILLIANCE);
	create("helmet", 10, 30, MATERIAL_IRON, 1, 0, 0);
	create("dented pot", 8, 10, MATERIAL_IRON, 1, 0, 0);
	create("elven leather helm", 8, 3, MATERIAL_LEATHER, 1, 0, 0);
	create("fedora", 1, 3, MATERIAL_CLOTH, 0, 0, 0);
	create("orcish helm", 10, 30, MATERIAL_IRON, 1, 0, 0);

	addToMap("iron skull cap", helmets["orcish helm"]);
	addToMap("conical hat", helmets["cornuthaum"]);
	addToMap("leather hat", helmets["elven leather helm"]);
	addToMap("hard hat", helmets["dwarvish iron helm"]);
}
