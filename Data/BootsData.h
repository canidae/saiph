#ifndef _BOOTSDATA_H
#define	_BOOTSDATA_H

#include <map>
#include <string>
#include "ArmorData.h"

class BootsData : public ArmorData {
public:
	static std::map<std::string, BootsData*> boots;
	static void init();

	BootsData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
};

#endif	/* _BOOTSDATA_H */

