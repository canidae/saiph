#ifndef DATA_SHIELD_H
#define DATA_SHIELD_H

#include <map>
#include <string>
#include "Data/Armor.h"

namespace data {

	class Shield : public Armor {
	public:
		virtual ~Shield();

		static void init();
		static const std::map<const std::string, const Shield*>& shields();

	protected:
		Shield(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);

		static void addToMap(const std::string& name, const Shield* shield);

	private:
		static std::map<const std::string, const Shield*> _shields;

		static void create(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	};
}
#endif
