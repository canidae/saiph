#ifndef ANALYZER_WEAPON_H
#define ANALYZER_WEAPON_H

#include <map>
#include <string>
#include "Analyzers/Analyzer.h"

#define WEAPON_WIELD_PRIORITY 200
/* how many melee and ranged weapon stacks we want */
//#define WEAPON_COUNT_MELEE 2
// due to its integration with #tw weapon count is constrained to 2
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
		void onEvent(event::Event* const event);

	private:
		unsigned char _best_weapon;
		unsigned char _alt_weapon;
		unsigned char _potentially_best_weapon;
		std::map<unsigned char, int> _best_missiles;

		bool betterThanWhatWeGot(const Item& item);
		int calculateWeaponScore(const Item& item, int use, bool potential = false);
		void setBestWeapons();
	};
}
#endif
