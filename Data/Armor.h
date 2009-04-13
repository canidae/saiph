#ifndef _ARMORDATA_H
#define	_ARMORDATA_H

#include <map>
#include <string>
#include "Item.h"
#include "../Globals.h"

namespace data {
	class Armor : public Item {
	public:
		static std::map<std::string, Armor *> armors;
		const int slot;
		const int ac;
		const int mc;

		Armor(const std::string &name, int cost, int weight, int material, int slot, int ac, int mc, unsigned long long properties);
		virtual ~Armor() {}

		static void init();

	protected:
		static void addToMap(const std::string &name, Armor *armor);
	};
}
#endif
