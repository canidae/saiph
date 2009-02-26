#include "GlovesData.h"

GlovesData::GlovesData(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) : ArmorData(name, cost, weight, material, ARMOR_GLOVES, ac, mc, properties) {
}
