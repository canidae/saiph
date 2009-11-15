#ifndef ANALYZER_WEAPON_H
#define ANALYZER_WEAPON_H

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

		void wieldBestWeapon();
	};
}
#endif
