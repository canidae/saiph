#ifndef WEAPON_H
#define WEAPON_H
/* priorities */
#define WEAPON_WIELD_PRIORITY 475
/* messages */
#define WEAPON_WHAT_TO_WIELD "  What do you want to wield? "

#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Request.h"

/* struct for wielding weapons */
struct WeaponData {
	int beatitude; // beatitudes we'll accept
	int priority; // wield weapon with highest priority, priority += enchantment
	bool keep; // if we should drop weapons with lower priority than what we're wield
	std::string name;
};

class Saiph;

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

		void wieldWeapon();
};
#endif
