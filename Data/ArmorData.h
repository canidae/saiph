#ifndef _ARMORDATA_H
#define	_ARMORDATA_H

#include "ItemData.h"
#include "../Globals.h"

class ArmorData : public ItemData {
public:
	/* TODO: after materials get implemented, hindersCasting() and erodes() */
	const int slot;
	const int ac;
	const int mc;

	ArmorData(const std::string& name, int cost, int weight, int material, int slot, int ac, int mc, unsigned long long properties);
};
#endif
