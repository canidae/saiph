#ifndef WEAPON_H
#define WEAPON_H

#include <string>
#include <vector>
#include "Analyzer.h"
#include "../Request.h"

/* messages */
#define WEAPON_WHAT_TO_WIELD "  What do you want to wield? "

/* struct for wielding weapons */
struct WeaponData {
	int beatitude; // beatitudes we'll accept
	int priority; // wield weapon with highest priority, priority += enchantment
	int amount; // how many of this weapon we want
	bool keep; // if we should keep this weapon even if we find better weapons
	std::string name;
};

class Saiph;

namespace analyzer {
	class Weapon : public Analyzer {
	public:
		Weapon(Saiph *saiph);

		void parseMessages(const std::string &messages);
		bool request(const Request &request);

	private:
		Saiph *saiph;
		bool wield_weapon;
		std::vector<WeaponData> weapons;
		std::string command2;
		Request req;
		bool can_wield_weapon;
		bool last_polymorphed;

		void wieldWeapon();
	};
}
#endif
