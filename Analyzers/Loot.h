#ifndef ANALYZER_LOOT_H
#define ANALYZER_LOOT_H

#include <map>
#include <string>
#include "Analyzer.h"
#include "../Coordinate.h"

/* messages */
#define LOOT_SEVERAL_MORE_OBJECTS_HERE "  There are several more objects here.  "
#define LOOT_SEVERAL_OBJECTS_HERE "  There are several objects here.  "
#define LOOT_MANY_MORE_OBJECTS_HERE "  There are many more objects here.  "
#define LOOT_MANY_OBJECTS_HERE "  There are many objects here.  "

#define LOOT_GOT_BURDENED "  Your movements are slowed slightly because of your load.  "
#define LOOT_GOT_STRESSED "  You rebalance your load.  Movement is difficult.  "
#define LOOT_GOT_STRAINED "  You stagger under your heavy load.  Movement is very hard.  "
#define LOOT_GOT_OVERTAXED "  You can barely move a handspan with this load!  "

namespace analyzer {
	class Loot : public Analyzer {
	public:
		Loot();

		void analyze();
		void parseMessages(const std::string &messages);

	private:
		bool showing_pickup;
		bool showing_drop;
		std::map<Coordinate, int> stash_checked; // location, turn we checked stash
		Coordinate check_old_stash;

		void checkOldStash();
	};
}
#endif
