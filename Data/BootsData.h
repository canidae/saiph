#ifndef _BOOTSDATA_H
#define	_BOOTSDATA_H

#include <map>
#include <string>
#include "ArmorData.h"

class BootsData : public ArmorData {
public:
	static std::map<std::string, BootsData *> boots;

	BootsData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	virtual ~BootsData() {}

	static void init();

protected:
	static void addToMap(const std::string &name, BootsData *boots);

private:
	static void create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
};
#endif
