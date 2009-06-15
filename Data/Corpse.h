#ifndef DATA_CORPSE_H
#define DATA_CORPSE_H

#include <map>
#include <string>
#include "Food.h"
#include "Monster.h"

namespace data {
	class Corpse : public Food {
	public:
		static std::map<std::string, Corpse *> corpses;
		const int resistance_confer_probability;

		Corpse(const std::string &name, int cost, int weight, int material, unsigned long long properties, int nutrition, int eat_time, int eat_effects, int resistance_confer_probability);
		~Corpse() {}

		static void init();

	protected:
		static void addToMap(const std::string &name, Corpse *corpse);

	private:
		static void create(const std::string &name, int nutrition, int eat_effects, int resistance_confer_probability);
	};
}
#endif
