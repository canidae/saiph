#ifndef LOOT_H
/* defines */
#define LOOT_H
/* if a stash change within this amount of turns since we last visited it, don't revisit */
#define LOOT_REVISIT_STASH_TIME 1
/* priorities */
#define LOOT_VISIT_STASH_PRIORITY 50

/* forward declare */
class Loot;

/* includes */
#include <list>
#include <map>
#include <string>
#include "../Analyzer.h"
#include "../Coordinate.h"
#include "../Globals.h"
#include "../Point.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* explores loot locations and picks up? */
class Loot : public Analyzer {
	public:
		/* constructors */
		Loot(Saiph *saiph);

		/* methods */
		void command(string *command);
		void finish();
		void parseMessages(const string &messages);
		bool request(const Request &request);

	private:
		/* variables */
		Saiph *saiph;
		string action;
		bool update_inventory;
		map<int, map<int, map<Point, int> > > turn_last_changed;
		list<Point> visit;
		map<Coordinate, int> loot;
};
#endif
