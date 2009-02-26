#ifndef _GLOVESDATA_H
#define	_GLOVESDATA_H

#include <map>
#include <string>
#include "ArmorData.h"

class GlovesData : public ArmorData {
public:
	static std::map<std::string, GlovesData> gloves;

	GlovesData(const std::string &name = "", int cost = 0, int weight = 0, int material = 0, int ac = 0, int mc = 0, unsigned long long properties = 0);
};
#endif
