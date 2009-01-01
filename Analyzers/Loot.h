#ifndef LOOT_H
#define LOOT_H
/* messages */
#define LOOT_NAME_INDIVIDUAL_OBECT "  Name an individual object? [ynq] (q)  "
#define LOOT_ITEM_TO_NAME "  What do you want to name? "
#define LOOT_ITEM_TO_CALL "  What do you want to call? "
#define LOOT_WHAT_TO_NAME_ITEM "  What do you want to name this "
#define LOOT_WHAT_TO_CALL_ITEM "  Call "
#define LOOT_SEVERAL_MORE_OBJECTS_HERE "  There are several more objects here.  "
#define LOOT_SEVERAL_OBJECTS_HERE "  There are several objects here.  "
#define LOOT_MANY_MORE_OBJECTS_HERE "  There are many more objects here.  "
#define LOOT_MANY_OBJECTS_HERE "  There are many objects here.  "
#define LOOT_STOLE " stole "
#define LOOT_STEALS " steals "

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
	bool only_unknown_enchantment;
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
		std::map<unsigned char, ItemGroup> groups;
		std::map<unsigned char, std::string> call_items;
		std::map<unsigned char, std::string> name_items;
		Request req;

		void checkInventory();
		void checkStash();
		int dropItem(const Item &item);
		int pickupItem(const Item &item);
		int pickupOrDropItem(const Item &item, bool drop);
};
#endif
