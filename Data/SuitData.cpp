#include "SuitData.h"

SuitData::SuitData(const std::string& name, int cost, int weight, long long int properties, int ac, int mc) : ArmorData(name, cost, weight, properties, ARMOR_SUIT, ac, mc) {
}

//someArmorVector.push_back(new SuitData("gray dragon scale mail", 900, 40, PROPERTY_MAGICRES, 9, 0));
