#ifndef _SHIELDDATA_H
#define	_SHIELDDATA_H

#include <map>
#include <string>
#include "ArmorData.h"

class ShieldData : public ArmorData {
public:
	static std::map<std::string, ShieldData*> shields;

	ShieldData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
};

#endif	/* _SHIELDDATA_H */

