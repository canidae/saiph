#include "ArmorData.h"

ArmorData::ArmorData(const std::string& name, int cost, int weight, long long int properties, int ac, int slot, int mc) : ItemData(name, cost, weight, properties, ARMOR), slot(slot), ac(ac), mc(mc) {
}
