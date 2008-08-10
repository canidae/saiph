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
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Item.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* analyzer for handling weapons (picking up, dropping, wielding) */
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
		map<string, int> weapon; // name & amount we want
		map<string, int> weapon_accept_beatitude; // name & accepted beatitudes
		map<int, vector<string> > weapon_group; // used for grouping weapons
		map<int, int> weapon_group_total; // total amount of weapons in group
		vector<string> wield_weapon; // the order of which we'll wield weapons
		string command2;
		Request req;

		/* methods */
		int weaponWanted(const Item &item);
};
#endif
