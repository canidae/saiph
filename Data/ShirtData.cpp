#include "ShirtData.h"

using namespace std;

/* initialize static variables */
map<string, ShirtData> ShirtData::shirts;

ShirtData::ShirtData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : ArmorData(name, cost, weight, material, ARMOR_SHIRT, ac, mc, properties) {
	shirts[name] = *this;
}
