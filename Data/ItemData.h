#ifndef ITEMDATA_H
#define ITEMDATA_H

#include <map>
#include <string>
#include "../Globals.h"

/* properties */
#define PROPERTY_FIRE              ((unsigned long long) (1LL << 0))
#define PROPERTY_COLD              ((unsigned long long) (1LL << 1))
#define PROPERTY_SLEEP             ((unsigned long long) (1LL << 2))
#define PROPERTY_DISINT            ((unsigned long long) (1LL << 3))
#define PROPERTY_SHOCK             ((unsigned long long) (1LL << 4))
#define PROPERTY_POISON            ((unsigned long long) (1LL << 5))
#define PROPERTY_ACID              ((unsigned long long) (1LL << 6))
#define PROPERTY_STONE             ((unsigned long long) (1LL << 7))
#define PROPERTY_MAGICRES          ((unsigned long long) (1LL << 8))
#define PROPERTY_REFLECTION        ((unsigned long long) (1LL << 9))
#define PROPERTY_DISPLACEMENT      ((unsigned long long) (1LL << 10))
#define PROPERTY_INVISIBLE         ((unsigned long long) (1LL << 11))
#define PROPERTY_VISIBLE           ((unsigned long long) (1LL << 12))
#define PROPERTY_BRILLIANCE        ((unsigned long long) (1LL << 13))
#define PROPERTY_ESP               ((unsigned long long) (1LL << 14))
#define PROPERTY_STUPIDITY         ((unsigned long long) (1LL << 15))
#define PROPERTY_STRENGTH          ((unsigned long long) (1LL << 16))
#define PROPERTY_DEXTERITY         ((unsigned long long) (1LL << 17))
#define PROPERTY_FUMBLING          ((unsigned long long) (1LL << 18))
#define PROPERTY_JUMPING           ((unsigned long long) (1LL << 19))
#define PROPERTY_KICKING           ((unsigned long long) (1LL << 20))
#define PROPERTY_LEVITATION        ((unsigned long long) (1LL << 21))
#define PROPERTY_STEALTH           ((unsigned long long) (1LL << 22))
#define PROPERTY_VERYFAST          ((unsigned long long) (1LL << 23))
#define PROPERTY_WATERWALKING      ((unsigned long long) (1LL << 24))
#define PROPERTY_SLIPPERY          ((unsigned long long) (1LL << 25))
#define PROPERTY_MAGIC             ((unsigned long long) (1LL << 26))
#define PROPERTY_RANDOM_APPEARANCE ((unsigned long long) (1LL << 27))

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
	static std::map<std::string, ItemData*> items;
	const std::string name;
	const int base_cost;
	const int weight;
	const char item_class;
	const int material;
	const unsigned long long properties;

	ItemData(const std::string &name = "", int base_cost = 0, int weight = 0, char item_class = ILLEGAL_ITEM, int material = 0, unsigned long long properties = 0);
};
#endif
