#include "ArmorData.h"
#include "BootsData.h"
#include "CloakData.h"
#include "GlovesData.h"
#include "HelmetData.h"
#include "ShieldData.h"
#include "ShirtData.h"
#include "SuitData.h"

using namespace std;

/* initialize static variables */
map<string, ArmorData *> ArmorData::armors;

ArmorData::ArmorData(const std::string &name, int cost, int weight, int material, int slot, int ac, int mc, unsigned long long properties) : ItemData(name, cost, weight, ARMOR, material, properties), slot(slot), ac(ac), mc(mc) {
}

void ArmorData::addToMap(const std::string &name, ArmorData *armor) {
	ArmorData::armors[name] = armor;
	ItemData::addToMap(name, armor);
}

void ArmorData::init() {
	BootsData::init();
	CloakData::init();
	GlovesData::init();
	HelmetData::init();
	ShieldData::init();
	ShirtData::init();
	SuitData::init();
	// TODO: fill out as more data is typed in
}
