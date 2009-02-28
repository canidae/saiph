#ifndef _CLOAKDATA_H
#define	_CLOAKDATA_H

#include <map>
#include <string>
#include "ArmorData.h"

class CloakData : public ArmorData {
public:
	static std::map<std::string, CloakData *> cloaks;

	CloakData(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	virtual ~CloakData() {}

	static void init();

protected:
	static void addToMap(const std::string &name, CloakData *cloak);

private:
	static void create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
};
#endif
