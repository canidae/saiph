#ifndef _CLOAKDATA_H
#define	_CLOAKDATA_H

#include <map>
#include <string>
#include "Armor.h"

namespace data {
	class Cloak : public Armor {
	public:
		static std::map<std::string, Cloak *> cloaks;

		Cloak(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
		virtual ~Cloak() {}

		static void init();

	protected:
		static void addToMap(const std::string &name, Cloak *cloak);

	private:
		static void create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	};
}
#endif
