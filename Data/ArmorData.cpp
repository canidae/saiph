#include "ArmorData.h"

ArmorData::ArmorData(const std::string& name, int cost, int weight, int material, int slot, int ac, int mc, unsigned long long properties) : ItemData(name, cost, weight, ARMOR, material, properties), slot(slot), ac(ac), mc(mc) {
}
