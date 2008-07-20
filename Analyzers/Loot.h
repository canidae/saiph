#ifndef LOOT_H
/* defines */
#define LOOT_H
/* priorities */
#define LOOT_VISIT_STASH_PRIORITY 50
/* how often we should check inventory */
#define LOOT_CHECK_INVENTORY_INTERVAL 1000

/* forward declare */
class Loot;

/* includes */
#include <list>
#include <string>
#include "../Analyzer.h"
#include "../Coordinate.h"
#include "../Globals.h"
#include "../Point.h"
#include "../Request.h"
#include "../Saiph.h"

/* LootStash */
class LootStash : public Coordinate {
	public:
		/* variables */
		int priority;

		/* constructors */
		LootStash(const Coordinate &coordinate, int priority) : Coordinate(coordinate), priority(priority) {}
};

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
		int last_turn_inventory_check;
		list<Point> visit;
		list<LootStash> loot;
};
#endif
