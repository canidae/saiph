#ifndef DOORANALYZER_H
/* defines */
#define DOORANALYZER_H
#define DO_MAX_DOORS 16
/* messages */
#define DO_CLOSES "The door closes."
#define DO_KICK_FAIL "WHAMMM!!!"
#define DO_KICK_OPEN "As you kick the door, it crashes open!"
#define DO_LOCKED "This door is locked."
#define DO_NO_DOOR "You see no door there."
#define DO_OPENS "The door opens."
#define DO_SHOP_ON_LEVEL1 "You hear someone cursing shoplifters."
#define DO_SHOP_ON_LEVEL2 "You hear the chime of a cash register."
#define DO_SHOP_ON_LEVEL3 "You hear Neiman and Marcus arguing!"
#define DO_RESISTS "The door resists!"
/* actions */
#define DO_NOTHING 0
#define DO_CHOOSE_DIRECTION 1
#define DO_KICK_DOOR 2
#define DO_OPEN_DOOR 3
#define DO_PICK_DOOR 4
#define DO_SEEK_DOOR 5
/* priorities */
#define DO_KICK_PRIORITY 10
#define DO_PICK_PRIORITY 20
#define DO_OPEN_PRIORITY 30

/* forward declare */
class DoorAnalyzer;

/* includes */
#include <string>
#include "../Saiph.h"

/* namespace */
using namespace std;

struct DO_Door {
	int row;
	int col;
	bool locked;
	bool shopkeeper;
};

/* analyzes the map and finds somewhere to explore */
class DoorAnalyzer : public Analyzer {
	public:
		/* constructors */
		DoorAnalyzer(Saiph *saiph);

		/* methods */
		void analyze(int row, int col, unsigned char symbol);
		void command(string *command);
		int finish();
		int parseMessages(string *messages);

	private:
		/* variables */
		Saiph *saiph;
		DO_Door doors[MAX_BRANCHES][MAX_DUNGEON_DEPTH][DO_MAX_DOORS];
		bool shop_on_level[MAX_BRANCHES][MAX_DUNGEON_DEPTH];
		int action;
		unsigned char action_direction;
		int action_door;
};
#endif
