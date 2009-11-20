#ifndef ANALYZER_WEAPON_H
#define ANALYZER_WEAPON_H

#include <map>
#include <string>
#include "Analyzer.h"

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

		int calculateWeaponScore(const Item& item);
		void setBestWeapon();
	};
}
#endif
