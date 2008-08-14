#ifndef LOOT_H
/* defines */
#define LOOT_H
/* priorities */
#define LOOT_LOOT_STASH_PRIORITY 450
#define LOOT_VISIT_STASH_PRIORITY 250
/* messages */
#define LOOT_SEVERAL_MORE_OBJECTS_HERE "  There are several more objects here.  "
#define LOOT_SEVERAL_OBJECTS_HERE "  There are several objects here.  "
#define LOOT_MANY_MORE_OBJECTS_HERE "  There are many more objects here.  "
#define LOOT_MANY_OBJECTS_HERE "  There are many objects here.  "
#define LOOT_STOLE " stole "

/* forward declare */
class Loot;

/* includes */
#include <map>
#include <string>
#include <sstream>
#include "../Analyzer.h"
#include "../Coordinate.h"
#include "../Globals.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* struct for items we want */
struct ItemWanted {
	int amount;
	int beatitude;
};

/* struct for groups */
struct ItemGroup {
	int amount;
	vector<string> items;
};

/* visit loot locations, open & close loot menus */
class Loot : public Analyzer {
	public:
		/* constructors */
		Loot(Saiph *saiph);

		/* methods */
		void analyze();
		void complete();
		void parseMessages(const string &messages);
		bool request(const Request &request);

	private:
		/* variables */
		Saiph *saiph;
		bool dirty_inventory;
		bool dirty_stash;
		bool showing_inventory;
		bool showing_pickup;
		map<Coordinate, int> visit_stash; // location, turn_changed
		map<string, ItemWanted> items;
		map<int, ItemGroup> groups;
		Request req;

		/* private methods */
		int pickupItem(const Item &item);
};
#endif
