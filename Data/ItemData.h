#ifndef ITEMDATA_H
#define ITEMDATA_H

#include <map>
#include <string>
#include "../Globals.h"

/* material */
#define MATERIAL_WAX         ((unsigned int)(1 << 0))
#define MATERIAL_VEGGY       ((unsigned int)(1 << 1))
#define MATERIAL_FLESH       ((unsigned int)(1 << 2))
#define MATERIAL_PAPER       ((unsigned int)(1 << 3))
#define MATERIAL_CLOTH       ((unsigned int)(1 << 4))
#define MATERIAL_LEATHER     ((unsigned int)(1 << 5))
#define MATERIAL_WOOD        ((unsigned int)(1 << 6))
#define MATERIAL_BONE        ((unsigned int)(1 << 7))
#define MATERIAL_DRAGON_HIDE ((unsigned int)(1 << 8))
#define MATERIAL_IRON        ((unsigned int)(1 << 9))
#define MATERIAL_METAL       ((unsigned int)(1 << 10))
#define MATERIAL_COPPER      ((unsigned int)(1 << 11))
#define MATERIAL_SILVER      ((unsigned int)(1 << 12))
#define MATERIAL_GOLD        ((unsigned int)(1 << 13))
#define MATERIAL_PLATINUM    ((unsigned int)(1 << 14))
#define MATERIAL_MITHRIL     ((unsigned int)(1 << 15))
#define MATERIAL_PLASTIC     ((unsigned int)(1 << 16))
#define MATERIAL_GLASS       ((unsigned int)(1 << 17))
#define MATERIAL_GEMSTONE    ((unsigned int)(1 << 18))
#define MATERIAL_MINERAL     ((unsigned int)(1 << 19))
#define MATERIAL_LIQUID      ((unsigned int)(1 << 20))

class ItemData {
public:
	static std::map<std::string, ItemData *> items;
	const std::string name;
	const int base_cost;
	const int weight;
	const char item_class;
	const int material;
	const unsigned long long properties;

	ItemData(const std::string &name, int base_cost, int weight, char item_class, int material, unsigned long long properties);
	virtual ~ItemData() {}

	static void init();
	static void destroy();

protected:
	static void addToMap(const std::string &name, ItemData *item);
};
#endif
