#ifndef LOOT_H
/* defines */
#define LOOT_H
/* used to help pick up a single item */
#define GET_SINGLE_ITEM "%7s %127[^\t\n]"
/* priorities */
#define LOOT_VISIT_STASH_PRIORITY 50
/* how often we should check inventory */
#define LOOT_CHECK_INVENTORY_INTERVAL 1000
/* messages */
#define LOOT_LITTLE_LIFTING "You have a little trouble lifting"
#define LOOT_EXTREME_LIFTING "You have extreme difficulty lifting"
#define LOOT_MUCH_LIFTING "You have much trouble lifting"

/* forward declare */
class Loot;

/* includes */
#include <list>
#include <map>
#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Coordinate.h"
#include "../Globals.h"
#include "../Item.h"
#include "../Point.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* Stash */
struct Stash {
	unsigned char top_item;
	vector<Item> items;
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
		bool request(const Request &request);

	private:
		/* variables */
		Saiph *saiph;
		string action;
		bool check_item[UCHAR_MAX + 1];
		Stash stashes[MAX_BRANCHES][MAX_DUNGEON_DEPTH][MAP_ROW_END + 1][MAP_COL_END + 1];
		list<Coordinate> stash_locations;
		list<Point> visit;
		list<Request> pickup;
		map<unsigned char, Item> inventory;
		int last_turn_inventory_check;
		Announce announce;

		/* methods */
		Item parseMessageItem(const string &message);
};
#endif
