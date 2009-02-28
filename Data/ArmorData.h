#ifndef _ARMORDATA_H
#define	_ARMORDATA_H

#include <map>
#include <string>
#include "ItemData.h"
#include "../Globals.h"

class ArmorData : public ItemData {
public:
	static std::map<std::string, ArmorData *> armors;
	const int slot;
	const int ac;
	const int mc;

	ArmorData(const std::string &name, int cost, int weight, int material, int slot, int ac, int mc, unsigned long long properties);
	virtual ~ArmorData() {}

	static void init();

protected:
	static void addToMap(const std::string &name, ArmorData *armor);
};
#endif
