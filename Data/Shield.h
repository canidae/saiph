#ifndef _SHIELDDATA_H
#define	_SHIELDDATA_H

#include <map>
#include <string>
#include "Armor.h"

namespace data {
	class Shield : public Armor {
	public:
		static std::map<std::string, Shield *> shields;

		Shield(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
		virtual ~Shield() {}

		static void init();

	protected:
		static void addToMap(const std::string &name, Shield *shield);

	private:
		static void create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	};
}
#endif
