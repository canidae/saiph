#ifndef _CORPSEDATA_H
#define	_CORPSEDATA_H

#include <map>
#include <string>
#include "FoodData.h"
#include "MonsterData.h"

class CorpseData : public FoodData {
public:
	static std::map<std::string, CorpseData *> corpses;

	CorpseData(const std::string &name, int cost, int weight, int material, unsigned long long properties, int nutrition, int eat_time, int eat_effects, int resistance_confer_probability);
	~CorpseData() {}

	static void init();

protected:
	static void addToMap(const std::string &name, CorpseData *corpse);

private:
	static void create(const std::string &name, int nutrition, int eat_effects, int resistance_confer_probability);
};
#endif
