#ifndef _SHIRTDATA_H
#define	_SHIRTDATA_H

#include <map>
#include <string>
#include "ArmorData.h"

class ShirtData : public ArmorData {
public:
	static std::map<std::string, ShirtData *> shirts;

	ShirtData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	virtual ~ShirtData() {}

	static void init();

protected:
	static void addToMap(const std::string &name, ShirtData *shirt);

private:
	static void create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
};
#endif
