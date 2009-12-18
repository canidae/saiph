#ifndef DATA_CORPSE_H
#define DATA_CORPSE_H

#include <map>
#include <string>
#include "Food.h"
#include "Monster.h"

namespace data {

	class Corpse : public Food {
	public:
		virtual ~Corpse();

		static void init();
		static const std::map<const std::string, const Corpse*>& corpses();
		int conferProbability() const;

	protected:
		Corpse(const std::string& name, int cost, int weight, int material, unsigned long long properties, int nutrition, int time, int effects, int confer_probability);

		static void addToMap(const std::string& name, const Corpse* corpse);

	private:
		static std::map<const std::string, const Corpse*> _corpses;
		const int _confer_probability;

		static void create(const std::string& name, int nutrition, int effects, int confer_probability);
	};
}
#endif
