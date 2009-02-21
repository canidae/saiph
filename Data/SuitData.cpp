#include "SuitData.h"

SuitData::SuitData(const std::string& name, int cost, int weight, bool magic, int ac, unsigned int properties) : ArmorData(name, cost, weight, magic), ac(ac), properties(properties) {
}

//someArmorVector.push_back(new SuitData("gray dragon scale mail", 900, 40, true, 9, PROPERTY_MAGICRES));