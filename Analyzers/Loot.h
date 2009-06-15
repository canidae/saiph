#ifndef ANALYZER_LOOT_H
#define ANALYZER_LOOT_H

#include <map>
#include <string>
#include <sstream>
#include <vector>
#include "Analyzer.h"
#include "../Coordinate.h"
#include "../Item.h"
#include "../Request.h"

/* messages */
#define LOOT_SEVERAL_MORE_OBJECTS_HERE "  There are several more objects here.  "
#define LOOT_SEVERAL_OBJECTS_HERE "  There are several objects here.  "
#define LOOT_MANY_MORE_OBJECTS_HERE "  There are many more objects here.  "
#define LOOT_MANY_OBJECTS_HERE "  There are many objects here.  "

/* FIXME: move to own actions */
#define LOOT_NAME_INDIVIDUAL_OBECT "  Name an individual object? [ynq] (q)  "
#define LOOT_ITEM_TO_NAME "  What do you want to name? "
#define LOOT_ITEM_TO_CALL "  What do you want to call? "
#define LOOT_WHAT_TO_NAME_ITEM "  What do you want to name this "
#define LOOT_WHAT_TO_CALL_ITEM "  Call "

namespace analyzer {
	class Loot : public Analyzer {
	public:
		Loot();

		void analyze();
		void complete();
		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		bool dirty_stash;
		bool showing_inventory;
		bool showing_pickup;
		bool showing_drop;
		std::map<Coordinate, int> visit_stash; // location, turn_changed
		Coordinate visit_old_stash;

		void checkStash();
		int dropItem(const Item &item);
		int pickupItem(const Item &item);
		int pickupOrDropItem(const Item &item, bool drop);
		void visitOldStash();
	};
}
#endif
