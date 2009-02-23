#ifndef _SUITDATA_H
#define	_SUITDATA_H

#include "ArmorData.h"
#include "../Globals.h"

class SuitData : public ArmorData {
public:
	SuitData(const std::string& name, int cost, int weight, long long int properties, int ac, int mc);
};
#endif
