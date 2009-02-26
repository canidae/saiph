#ifndef _GLOVESDATA_H
#define	_GLOVESDATA_H

#include "ArmorData.h"

class GlovesData : public ArmorData {
public:
	GlovesData(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
};

#endif	/* _GLOVESDATA_H */

