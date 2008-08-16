#ifndef WEAPON_H
/* defines */
#define WEAPON_H
/* priorities */
#define WEAPON_PICKUP_PRIORITY 425
#define WEAPON_WIELD_PRIORITY 475
/* messages */
#define WEAPON_WHAT_TO_WIELD "  What do you want to wield? "

/* forward declare */
class Weapon;

/* includes */
#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* struct for wielding weapons */
struct WieldWeapon {
	int beatitude;
	string name;
};

/* analyzer for wielding weapons */
class Weapon : public Analyzer {
	public:
		/* constructors */
		Weapon(Saiph *saiph);

		/* methods */
		void analyze();
		void parseMessages(const string &messages);
		bool request(const Request &request);

	private:
		/* variables */
		Saiph *saiph;
		bool wield_more;
		vector<WieldWeapon> wield; // the order of which we'll wield weapons
		string command2;
		Request req;

		/* methods */
		void wieldWeapon();
};
#endif
