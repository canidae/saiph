#include "SuitData.h"

using namespace std;

/* initialize static variables */
map<string, SuitData> SuitData::suits;

SuitData::SuitData(const std::string &name = "", int cost = 0, int weight = 0, int material = 0, int ac = 0, int mc = 0, unsigned long long properties = 0) : ArmorData(name, cost, weight, material, ARMOR_SUIT, ac, mc, properties) {
	suits[name] = *this;
}
