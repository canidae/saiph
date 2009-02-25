#include "ArmorData.h"

ArmorData::ArmorData(const std::string& name, int cost, int weight, int material, int slot, int ac, int mc, long long int properties) : ItemData(name, cost, weight, ARMOR, material, properties), slot(slot), ac(ac), mc(mc) {
}
