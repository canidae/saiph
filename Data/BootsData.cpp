#include "BootsData.h"

using namespace std;

/* initialize static variables */
map<string, BootsData*> BootsData::boots;

BootsData::BootsData(const std::string &name = "", int cost = 0, int weight = 0, int material = 0, int ac = 0, int mc = 0, unsigned long long properties = 0) : ArmorData(name, cost, weight, material, ARMOR_SUIT, ac, mc, properties) {
	boots[name] = this;
}

