#include "CloakData.h"

using namespace std;

/* initialize static variables */
map<string, CloakData*> CloakData::cloaks;

CloakData::CloakData(const std::string &name = "", int cost = 0, int weight = 0, int material = 0, int ac = 0, int mc = 0, unsigned long long properties = 0) : ArmorData(name, cost, weight, material, ARMOR_SUIT, ac, mc, properties) {
}

namespace { //an anonymous namespace, to avoid linker errors from the various create()s
	inline void stuffInMaps(const std::string& name, CloakData* foo) {
		CloakData::cloaks[name] = foo;
		ArmorData::armors[name] = foo;
		ItemData::items[name] = foo;
	}
	
	inline void create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties) {
		CloakData* foo = new CloakData(name, cost, weight, material, ac, mc, properties);
		stuffInMaps(foo->name, foo);
	}
}

void CloakData::init() {
	//     name                       cost  weight  material          ac mc properties
	create("mummy wrapping",             2,      3, MATERIAL_CLOTH,    0, 1, PROPERTY_VISIBLE);
	create("orcish cloak",              40,     10, MATERIAL_CLOTH,    0, 2, 0);
	create("dwarvish cloak",            50,     10, MATERIAL_CLOTH,    0, 2, 0);
	create("leather cloak",             40,     15, MATERIAL_LEATHER,  1, 1, 0);
	create("oilskin cloak",             50,     10, MATERIAL_CLOTH,    1, 3, PROPERTY_SLIPPERY);
	create("alchemy smock",             50,     10, MATERIAL_CLOTH,    1, 1, PROPERTY_POISON | PROPERTY_ACID | PROPERTY_MAGIC);
	create("elven cloak",               60,     10, MATERIAL_CLOTH,    1, 3, PROPERTY_STEALTH | PROPERTY_MAGIC);
	create("robe",                      50,     15, MATERIAL_CLOTH,    2, 3, PROPERTY_CASTING_BONUS | PROPERTY_MAGIC);
	create("cloak of magic resistance", 60,     10, MATERIAL_CLOTH,    1, 3, PROPERTY_MAGICRES | PROPERTY_MAGIC);
	create("cloak of displacement",     50,     10, MATERIAL_CLOTH,    1, 2, PROPERTY_DISPLACEMENT | PROPERTY_MAGIC);
	create("cloak of invisibility",     60,     10, MATERIAL_CLOTH,    1, 2, PROPERTY_INVISIBLE | PROPERTY_MAGIC);
	create("cloak of protection",       50,     10, MATERIAL_CLOTH,    3, 3, PROPERTY_MAGIC);

	//constant appearances
	stuffInMaps("coarse mantelet", cloaks["orcish cloak"]);
	stuffInMaps("hooded cloak", cloaks["dwarvish cloak"]);
	stuffInMaps("slippery cloak", cloaks["oilskin cloak"]);
	stuffInMaps("apron", cloaks["alchemy smock"]);
	stuffInMaps("faded pall", cloaks["elven cloak"]);

	//TODO: randomized appearance
	//tattered cape, ornamental cope, opera cloak, piece of cloth
}
