#ifndef _WEAPONDATA_H
#define	_WEAPONDATA_H

#include <map>
#include <string>
#include "Item.h"
#include "Attack.h"
#include "../Globals.h"

namespace data {

	class Weapon : public Item {
	public:
		static std::map<std::string, Weapon *> weapons;
		bool one_handed;
		int alignment;
		Attack attack_small[3];
		Attack attack_large[3];

		Weapon(const std::string &name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2);
		virtual ~Weapon() {}

		static void init();

	protected:
		static void addToMap(const std::string &name, Weapon *weapon);
	};
}
#endif
