#ifndef _SUITDATA_H
#define	_SUITDATA_H

#include <map>
#include <string>
#include "ArmorData.h"

class SuitData : public ArmorData {
public:
	static std::map<std::string, SuitData*> suits;

	SuitData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
};
#endif
