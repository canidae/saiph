#ifndef LOOT_H
#define LOOT_H
/* priorities */
#define LOOT_LOOT_STASH_PRIORITY 450
#define LOOT_DROP_ITEMS_PRIORITY 225
#define LOOT_VISIT_STASH_PRIORITY 250
#define LOOT_PROTECT_STAIR_STASH_PRIORITY 550
/* messages */
#define LOOT_SEVERAL_MORE_OBJECTS_HERE "  There are several more objects here.  "
#define LOOT_SEVERAL_OBJECTS_HERE "  There are several objects here.  "
#define LOOT_MANY_MORE_OBJECTS_HERE "  There are many more objects here.  "
#define LOOT_MANY_OBJECTS_HERE "  There are many objects here.  "
#define LOOT_STOLE " stole "

#include <map>
#include <string>
#include <sstream>
#include <vector>
#include "../Analyzer.h"
#include "../Coordinate.h"
#include "../Item.h"
#include "../Request.h"

/* struct for items we want */
struct ItemWanted {
	int amount;
	int beatitude;
};

/* struct for groups */
struct ItemGroup {
	int amount;
	std::vector<std::string> items;
};

class Saiph;

class Loot : public Analyzer {
	public:
		Loot(Saiph *saiph);

		void analyze();
		void complete();
		void fail();
		void parseMessages(const std::string &messages);
		bool request(const Request &request);

	private:
		Saiph *saiph;
		bool dirty_inventory;
		bool dirty_stash;
		bool showing_inventory;
		bool showing_pickup;
		bool showing_drop;
		std::map<Coordinate, int> visit_stash; // location, turn_changed
		std::map<std::string, ItemWanted> items;
		std::map<int, ItemGroup> groups;
		Request req;

		void checkInventory();
		void checkStash();
		int unwantedItem(const Item &item);
		int wantedItem(const Item &item);
};
#endif
