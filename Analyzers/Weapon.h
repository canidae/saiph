#ifndef WEAPON_H
#define WEAPON_H
/* priorities */
#define WEAPON_PICKUP_PRIORITY 425
#define WEAPON_WIELD_PRIORITY 475
/* messages */
#define WEAPON_WHAT_TO_WIELD "  What do you want to wield? "

#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Request.h"

/* struct for wielding weapons */
struct WieldWeapon {
	int beatitude;
	std::string name;
};

class Saiph;

class Weapon : public Analyzer {
	public:
		Weapon(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);
		bool request(const Request &request);

	private:
		Saiph *saiph;
		bool wield_more;
		std::vector<WieldWeapon> wield; // the order of which we'll wield weapons
		std::string command2;
		Request req;

		void wieldWeapon();
};
#endif
