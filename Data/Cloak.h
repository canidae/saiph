#ifndef DATA_CLOAK_H
#define DATA_CLOAK_H

#include <map>
#include <string>
#include "Armor.h"

namespace data {

	class Cloak : public Armor {
	public:
		virtual ~Cloak();

		static void init();
		static const std::map<const std::string, const Cloak*>& cloaks();

	protected:
		Cloak(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);

		static void addToMap(const std::string& name, const Cloak* cloak);

	private:
		static std::map<const std::string, const Cloak*> _cloaks;

		static void create(const std::string& name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	};
}
#endif
