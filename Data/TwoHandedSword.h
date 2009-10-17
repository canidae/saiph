#ifndef DATA_TWO_HANDED_SWORDS_H
#define DATA_TWO_HANDED_SWORDS_H

#include <map>
#include <string>
#include "Weapon.h"

namespace data {

	class TwoHandedSword : public Weapon {
	public:
		static std::map<std::string, TwoHandedSword *> two_handed_swords;

		TwoHandedSword(const std::string &name, int cost, int weight, char item_class, int material, unsigned long long properties, bool one_handed, int alignment, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2);

		virtual ~TwoHandedSword() {
		}

		static void init();

	protected:
		static void addToMap(const std::string &name, TwoHandedSword *two_handed_sword);

	private:
		static void create(const std::string &name, int cost, int weight, const Attack &as0, const Attack &as1, const Attack &as2, const Attack &al0, const Attack &al1, const Attack &al2, int material, char item_class, unsigned long long properties, bool one_handed, int alignment);
	};
}
#endif
