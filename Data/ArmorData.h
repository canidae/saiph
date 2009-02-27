#ifndef _ARMORDATA_H
#define	_ARMORDATA_H

#include <map>
#include <string>
#include "ItemData.h"
#include "../Globals.h"

class ArmorData : public ItemData {
public:
	static std::map<std::string, ArmorData*> armors;
	const int slot;
	const int ac;
	const int mc;

	ArmorData(const std::string &name = "", int cost = 0, int weight = 0, int material = 0, int slot = 0, int ac = 0, int mc = 0, unsigned long long properties = 0);

	/* TODO: after materials get implemented, hindersCasting() and erodes() */
};
#endif
