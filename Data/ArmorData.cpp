#include "ArmorData.h"

ArmorData::ArmorData(const std::string& name, int cost, int weight, bool magic, int ac, int slot, int mc, unsigned int properties) : ItemData(name, cost, weight, magic, ARMOR), slot(slot), ac(ac), mc(mc), properties(properties) {
}
