#ifndef WEAPON_H
#define WEAPON_H
/* priorities */
#define WEAPON_PICKUP_PRIORITY 425
#define WEAPON_WIELD_PRIORITY 475

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
		std::vector<WieldWeapon> wield; // the order of which we'll wield weapons
		Request req;

		void wieldWeapon();
};
#endif
