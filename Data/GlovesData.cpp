#include "GlovesData.h"

using namespace std;

/* initialize static variables */
map<string, GlovesData *> GlovesData::gloves;

GlovesData::GlovesData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : ArmorData(name, cost, weight, material, ARMOR_GLOVES, ac, mc, properties) {
}

void GlovesData::addToMap(const std::string &name, GlovesData *gloves) {
	GlovesData::gloves[name] = gloves;
	ArmorData::addToMap(name, gloves);
}

void GlovesData::create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
	addToMap(name, new GlovesData(name, cost, weight, material, ac, mc, properties));
}

void GlovesData::init() {
}
