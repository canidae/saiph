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
}
