#include "SuitData.h"

using namespace std;

/* initialize static variables */
map<string, SuitData *> SuitData::suits;

SuitData::SuitData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : ArmorData(name, cost, weight, material, ARMOR_SUIT, ac, mc, properties) {
}

void SuitData::addToMap(const std::string &name, SuitData *suit) {
	SuitData::suits[name] = suit;
	ArmorData::addToMap(name, suit);
}

void SuitData::create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
	addToMap(name, new SuitData(name, cost, weight, material, ac, mc, properties));
}

void SuitData::init() {
}
