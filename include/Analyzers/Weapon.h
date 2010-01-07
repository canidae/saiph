#ifndef ANALYZER_WEAPON_H
#define ANALYZER_WEAPON_H

#include <map>
#include <string>
#include "Analyzers/Analyzer.h"

#define WEAPON_WIELD_PRIORITY 200
/* how many melee and ranged weapon stacks we want */
#define WEAPON_COUNT_MELEE 2
#define WEAPON_COUNT_RANGE 5

namespace data {
	class Weapon;
}

class Item;

namespace analyzer {

	class Weapon : public Analyzer {
	public:
		Weapon();

		void analyze();
		void onEvent(event::Event * const event);

	private:
		unsigned char _wield_weapon;
		std::map<unsigned char, int> _melee_weapons;
		std::map<unsigned char, int> _range_weapons;

		bool betterThanWhatWeGot(const Item& item);
		int calculateWeaponScore(const Item& item, const data::Weapon* weapon);
		bool isRangedWeapon(const data::Weapon* weapon);
		void setBestWeapons();
	};
}
#endif
