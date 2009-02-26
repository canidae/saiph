#include "GlovesData.h"

using namespace std;

/* initialize static variables */
map<string, GlovesData> GlovesData::gloves;

GlovesData::GlovesData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : ArmorData(name, cost, weight, material, ARMOR_GLOVES, ac, mc, properties) {
	gloves[name] = *this;
}
