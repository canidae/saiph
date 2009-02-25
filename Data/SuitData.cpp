#include "SuitData.h"

SuitData::SuitData(const std::string& name, int cost, int weight, int material, int ac, int mc, long long int properties) : ArmorData(name, cost, weight, material, ARMOR_SUIT, ac, mc, properties) {
}

