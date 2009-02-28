#ifndef _CLOAKDATA_H
#define	_CLOAKDATA_H

#include <map>
#include <string>
#include "ArmorData.h"

class CloakData : public ArmorData {
public:
	static std::map<std::string, CloakData*> cloaks;
	static void init();

	CloakData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
};

#endif	/* _CLOAKDATA_H */

