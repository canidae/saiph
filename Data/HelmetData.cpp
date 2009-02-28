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
}
