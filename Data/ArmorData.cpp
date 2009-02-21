#include "ArmorData.h"

ArmorData::ArmorData(const std::string& name, int cost, int weight, bool magic, int ac, unsigned int properties) : ItemData(name, cost, weight, magic), ac(ac), properties(properties) {
}
