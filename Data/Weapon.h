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
		unsigned long long properties_wielded;
		int alignment;
		bool one_handed;
		Attack attack_small[3];
		Attack attack_large[3];

		Weapon(const std::string &name, int cost, int weight, char item_class, int material, unsigned long long properties, unsigned long long properties_wielded, int alignment, bool one_handed, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2);
		virtual ~Weapon() {}

		static void init();

	protected:
		static void addToMap(const std::string &name, Weapon *weapon);

	private:
		static void create(const std::string &name, int cost, int weight, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2, int material, char item_class, unsigned long long properties, unsigned long long properties_wielded, int alignment, bool one_handed);
		static void createSimple(const std::string &name, int cost, int weight, const Attack &as0, const Attack &as1, const Attack &al0, const Attack &al1, int material, char item_class, bool one_handed);
	};
}
#endif
