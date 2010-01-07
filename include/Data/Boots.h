#ifndef DATA_BOOTS_H
#define DATA_BOOTS_H

#include <map>
#include <string>
#include "Data/Armor.h"

namespace data {

	class Boots : public Armor {
	public:
		virtual ~Boots();

		static void init();
		static const std::map<const std::string, const Boots*>& boots();

	protected:
		Boots(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);

		static void addToMap(const std::string& name, const Boots* boots);

	private:
		static std::map<const std::string, const Boots*> _boots;

		static void create(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	};
}
#endif
