#ifndef _SHIRTDATA_H
#define	_SHIRTDATA_H

#include "ArmorData.h"

class ShirtData : public ArmorData {
public:
	ShirtData(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
};
#endif
