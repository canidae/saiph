#ifndef _HELMETDATA_H
#define	_HELMETDATA_H

#include <map>
#include <string>
#include "Armor.h"

namespace data {
	class Helmet : public Armor {
	public:
		static std::map<std::string, Helmet *> helmets;

		Helmet(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
		virtual ~Helmet() {}

		static void init();

	protected:
		static void addToMap(const std::string &name, Helmet *helmet);

	private:
		static void create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	};
}
#endif
