#include "ShirtData.h"
#include "../Globals.h"

ShirtData::ShirtData(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : ArmorData(name, cost, weight, material, ARMOR_SHIRT, ac, mc, properties) {
}
