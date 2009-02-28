#ifndef _SUITDATA_H
#define	_SUITDATA_H

#include <map>
#include <string>
#include "ArmorData.h"

class SuitData : public ArmorData {
public:
	static std::map<std::string, SuitData *> suits;

	SuitData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	virtual ~SuitData() {}

	static void init();

protected:
	static void addToMap(const std::string &name, SuitData *suit);

private:
	static void create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
};
#endif
