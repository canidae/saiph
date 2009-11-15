#ifndef ANALYZER_WEAPON_H
#define ANALYZER_WEAPON_H

#include <string>
#include "Analyzer.h"

/* messages */
#define WEAPON_WHAT_TO_WIELD "  What do you want to wield? "

class Item;

namespace analyzer {

	class Weapon : public Analyzer {
	public:
		Weapon();

		void parseMessages(const std::string& messages);
		void onEvent(event::Event * const event);

	private:
		bool _wield_weapon;

		void wantWeapon(const Item& item);
		void wieldWeapon();
	};
}
#endif
