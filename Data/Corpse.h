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
		const int& conferProbability() const;

	protected:
		Corpse(const std::string& name, const int& cost, const int& weight, const int& material, const unsigned long long& properties, const int& nutrition, const int& time, const int& effects, const int& confer_probability);

		static void addToMap(const std::string& name, const Corpse* corpse);

	private:
		static std::map<const std::string, const Corpse*> _corpses;
		const int _confer_probability;

		static void create(const std::string& name, const int& nutrition, const int& effects, const int& confer_probability);
	};
}
#endif
