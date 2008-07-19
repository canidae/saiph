#ifndef DOOR_H
/* defines */
#define DOOR_H
/* messages */
#define DOOR_CLOSES "The door closes."
#define DOOR_KICK_FAIL "WHAMMM!!!"
#define DOOR_KICK_OPEN "As you kick the door, it crashes open!"
#define DOOR_LOCKED "This door is locked."
#define DOOR_NO_DOOR "You see no door there."
#define DOOR_OPENS "The door opens."
#define DOOR_SHOP_ON_LEVEL1 "You hear someone cursing shoplifters."
#define DOOR_SHOP_ON_LEVEL2 "You hear the chime of a cash register."
#define DOOR_SHOP_ON_LEVEL3 "You hear Neiman and Marcus arguing!"
#define DOOR_RESISTS "The door resists!"
/* priorities */
#define DOOR_KICK_PRIORITY 35
#define DOOR_OPEN_PRIORITY 45
#define DOOR_PICK_PRIORITY 40
/* actions */
#define DOOR_KICK 4 // ctrl-d
#define DOOR_OPEN 'o'

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
