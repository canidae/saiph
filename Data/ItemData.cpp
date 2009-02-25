#include "ItemData.h"

ItemData::ItemData(const std::string& name, int base_cost, int weight, char item_class, int material, long long int properties) : name(name), base_cost(base_cost), weight(weight), item_class(item_class), material(material), properties(properties) {
}
