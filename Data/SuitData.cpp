#include "SuitData.h"

SuitData::SuitData(const std::string& name, int cost, int weight, bool magic, int ac, int mc, unsigned int properties) : ArmorData(name, cost, weight, magic, ARMOR_SUIT, ac, mc, properties) {
}

//someArmorVector.push_back(new SuitData("gray dragon scale mail", 900, 40, true, 9, 0, PROPERTY_MAGICRES));