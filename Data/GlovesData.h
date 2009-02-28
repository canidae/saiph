#ifndef _GLOVESDATA_H
#define	_GLOVESDATA_H

#include <map>
#include <string>
#include "ArmorData.h"

class GlovesData : public ArmorData {
public:
	static std::map<std::string, GlovesData *> gloves;

	GlovesData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	~GlovesData() {}

	static void init();

protected:
	static void addToMap(const std::string &name, GlovesData *gloves);

private:
	static void create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
};
#endif
