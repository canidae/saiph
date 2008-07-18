#ifndef LOOT_H
/* defines */
#define LOOT_H
/* used to help pick up a single item */
#define GET_SINGLE_ITEM "%7s %127[^\t\n]"
/* messages */
#define LOOT_LITTLE_LIFTING "You have a little trouble lifting"
#define LOOT_EXTREME_LIFTING "You have extreme difficulty lifting"
#define LOOT_MUCH_LIFTING "You have much trouble lifting"

/* forward declare */
class Loot;

/* includes */
#include <list>
#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Coordinate.h"
#include "../Globals.h"
#include "../Item.h"
#include "../Point.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* Stash */
class Stash {
	public:
		/* variables */
		vector<Item> items;
		unsigned char top_item;

		/* constructors */
		Stash() { top_item = ILLEGAL_ITEM; }
};

/* analyzes the map and finds somewhere to explore */
class Loot : public Analyzer {
	public:
		/* constructors */
		Loot(Saiph *saiph);

		/* methods */
		void command(string *command);
		void finish();
		void inspect(const Point &point);
		void parseMessages(string *messages);

	private:
		/* variables */
		Saiph *saiph;
		string action;
		bool check_item[UCHAR_MAX + 1];
		Stash stashes[MAX_BRANCHES][MAX_DUNGEON_DEPTH][MAP_ROW_END + 1][MAP_COL_END + 1];
		list<Coordinate> stash_locations;
		list<Point> visit;
		Announce announce;

		/* methods */
		void parseMessageItem(const string &message, vector<Item> *stash);
};
#endif
