#ifndef LOOT_H
/* defines */
#define LOOT_H
/* messages */
#define LOOT_LITTLE_LIFTING "You have a little trouble lifting"
#define LOOT_EXTREME_LIFTING "You have extreme difficulty lifting"
#define LOOT_MUCH_LIFTING "You have much trouble lifting"

/* forward declare */
class Loot;

/* includes */
#include <string>
#include "../Analyzer.h"
#include "../Coordinate.h"
#include "../Globals.h"
#include "../Item.h"
#include "../Point.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* struct */
struct Stash {
	vector<Item> items;
	unsigned char top_item;
};

/* analyzes the map and finds somewhere to explore */
class Loot : public Analyzer {
	public:
		/* constructors */
		Loot(Saiph *saiph);

		/* methods */
		void command(string *command);
		int finish();
		void inspect(const Point &point);
		int parseMessages(string *messages);

	private:
		/* variables */
		Saiph *saiph;
		string action;
		Stash stashes[MAX_BRANCHES][MAX_DUNGEON_DEPTH][MAP_ROW_END + 1][MAP_COL_END + 1];
		vector<Coordinate> stash_locations;

		/* methods */
		void parseMessageItem(const string &message, vector<Item> *stash);
};
#endif
