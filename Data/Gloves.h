#ifndef _GLOVESDATA_H
#define	_GLOVESDATA_H

#include <map>
#include <string>
#include "Armor.h"

namespace data {
	class Gloves : public Armor {
	public:
		static std::map<std::string, Gloves *> gloves;

		Gloves(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
		~Gloves() {}

		static void init();

	protected:
		static void addToMap(const std::string &name, Gloves *gloves);

	private:
		static void create(const std::string &name, int cost, int weight, int material, int ac, int mc, unsigned long long properties);
	};
}
#endif
