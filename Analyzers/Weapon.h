#ifndef WEAPON_H
/* defines */
#define WEAPON_H
/* priorities */
#define WEAPON_PICKUP_PRIORITY 425
/* used for finding thrown weapons */
#define WEAPON_NOT_CHECKED_THROWN_WEAPONS 0
#define WEAPON_NO_THROWN_WEAPONS 1

/* forward declare */
class Weapon;

/* includes */
#include <map>
#include <sstream>
#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Globals.h"
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
		map<string, int> weapon; // name & amount we want
		map<string, int> weapon_accept_beatitude; // name & accepted beatitudes
		map<int, vector<string> > weapon_group; // used for grouping weapons
		map<int, int> weapon_group_total; // total amount of weapons in group
		Saiph *saiph;
};
#endif
