#ifndef DATA_BOOTS_H
#define DATA_BOOTS_H

#include <map>
#include <string>
#include "Armor.h"

namespace data {

	class Boots : public Armor {
	public:
		static std::map<std::string, Boots*> boots;

		Boots(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);

		virtual ~Boots() {
		}

		static void init();

	protected:
		static void addToMap(const std::string& name, Boots* boots);

	private:
		static void create(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	};
}
#endif
