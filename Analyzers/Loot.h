#ifndef LOOT_H
/* defines */
#define LOOT_H
/* priorities */
#define LOOT_DISCOVER_STASH_PRIORITY 250

/* forward declare */
class Loot;

/* includes */
#include <map>
#include <string>
#include "../Analyzer.h"
#include "../Coordinate.h"
#include "../Globals.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* visit loot locations, open & close loot menus */
class Loot : public Analyzer {
	public:
		/* constructors */
		Loot(Saiph *saiph);

		/* methods */
		void analyze();
		void parseMessages(const string &messages);
		bool request(const Request &request);

	private:
		/* variables */
		Saiph *saiph;
		bool dirty_inventory;
		bool listing_stuff;
		map<Coordinate, int> discover_stash; // location, turn_changed
		map<Coordinate, int> visit_stash; // location, priority
};
#endif
