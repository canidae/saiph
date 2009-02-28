#include "ArmorData.h"
#include "ShirtData.h"

using namespace std;

/* initialize static variables */
map<string, ArmorData*> ArmorData::armors;

ArmorData::ArmorData(const std::string &name, int cost, int weight, int material, int slot, int ac, int mc, unsigned long long properties) : ItemData(name, cost, weight, ARMOR, material, properties), slot(slot), ac(ac), mc(mc) {
}

void ArmorData::init() {
	ShirtData::init();
	BootsData::init();
	CloakData::init();
	//TODO: fill out as more data is typed in
}
