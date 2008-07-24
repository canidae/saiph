#ifndef DOOR_H
/* defines */
#define DOOR_H
/* priorities */
#define DOOR_KICK_PRIORITY 50
#define DOOR_OPEN_PRIORITY 150
#define DOOR_PICK_PRIORITY 100

/* forward declare */
class Door;

/* includes */
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Point.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* a DoorPoint. just a point with some more info */
class DoorPoint : public Point {
	public:
		/* variables */
		bool locked;
};

/* opening/picking/kicking doors is a multi turn affair.
 * we'll need a more complex way of storing an action */
struct DoorAction {
	unsigned char direction;
	unsigned char action;
	DoorPoint *dp;
};

/* analyzes the map and finds somewhere to explore */
class Door : public Analyzer {
	public:
		/* constructors */
		Door(Saiph *saiph);

		/* methods */
		void command(string *command);
		void finish();
		void inspect(const Point &point);
		void parseMessages(const string &messages);

	private:
		/* variables */
		Saiph *saiph;
		vector<DoorPoint> doors[MAX_BRANCHES][MAX_DUNGEON_DEPTH];
		bool shop_on_level[MAX_BRANCHES][MAX_DUNGEON_DEPTH];
		DoorAction da;
};
#endif
