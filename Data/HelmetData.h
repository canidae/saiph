#ifndef _HELMETDATA_H
#define	_HELMETDATA_H

#include <map>
#include <string>
#include "ArmorData.h"

class HelmetData : public ArmorData {
public:
	static std::map<std::string, HelmetData *> helmets;

	HelmetData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	virtual ~HelmetData() {}

	static void init();

protected:
	static void addToMap(const std::string &name, HelmetData *helmet);

private:
	static void create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
};
#endif
