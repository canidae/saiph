#ifndef SAIPH_H
/* defines */
#define SAIPH_H
/* debug */
#define COMMAND_DEBUG_NAME "[Command    ] "
#define ITEMTRACKER_DEBUG_NAME "[ItemTracker] "
#define MESSAGES_DEBUG_NAME "[Messages   ] "
#define REQUEST_DEBUG_NAME "[Request    ] "
#define SAIPH_DEBUG_NAME "[Saiph      ] "
/* pathing */
#define COST_CARDINAL 2
#define COST_DIAGONAL 3
#define COST_ICE 8 // slippery and risky, try to find a way around (don't try very hard, though)
#define COST_LAVA 512 // lava, hot!
#define COST_PET 16 // try not to move onto pets
#define COST_TRAP 128 // avoid traps
#define COST_WATER 256 // avoid water if possible
#define PATHING_QUEUE_SIZE 4096 // max amount of nodes in pathing_queue
/* messages */
#define SAIPH_GAIN_COLD_RES1 "  You feel full of hot air.  "
#define SAIPH_LOSE_COLD_RES1 "  You feel cooler!  "
#define SAIPH_GAIN_DISINTEGRATION_RES1 "  You feel very firm.  "
#define SAIPH_GAIN_DISINTEGRATION_RES2 "  You feel totally together, man.  "
#define SAIPH_GAIN_FIRE_RES1 "  You feel a momentary chill.  "
#define SAIPH_GAIN_FIRE_RES2 "  You be chillin'.  "
#define SAIPH_LOSE_FIRE_RES1 "  You feel warmer!  "
#define SAIPH_GAIN_POISON_RES1 "  You feel healthy.  "
#define SAIPH_GAIN_POISON_RES2 "  You feel especially healthy.  "
#define SAIPH_LOSE_POISON_RES1 "  You feel a little sick!  "
#define SAIPH_GAIN_SHOCK_RES1 "  Your health currently feels amplified!  "
#define SAIPH_GAIN_SHOCK_RES2 "  You feel grounded in reality.  "
#define SAIPH_LOSE_SHOCK_RES1 "  You feel conductive!  "
#define SAIPH_GAIN_SLEEP_RES1 "  You feel wide awake.  "
#define SAIPH_LOSE_SLEEP_RES1 "  You feel tired!  "
#define SAIPH_GAIN_TELEPATHY1 "  You feel a strange mental acuity.  "
#define SAIPH_LOSE_TELEPATHY1 "  Your senses fail!  "
#define SAIPH_GAIN_TELEPORT_CONTROL1 "  You feel in control of yourself.  "
#define SAIPH_GAIN_TELEPORT_CONTROL2 "  You feel centered in your personal space.  "
#define SAIPH_GAIN_TELEPORTITIS1 "  You feel very jumpy.  "
#define SAIPH_GAIN_TELEPORTITIS2 "  You feel diffuse.  "
#define SAIPH_LOSE_TELEPORTITIS1 "  You feel less jumpy.  "
#define SAIPH_FEEL_FEVERISH "  You feel feverish.  "
#define SAIPH_FEEL_PURIFIED "  You feel purified.  "
#define SAIPH_HURT_LEFT_LEG "  Your left leg is in no shape for kicking.  "
#define SAIPH_HURT_RIGHT_LEG "  Your right leg is in no shape for kicking.  "
#define SAIPH_LEG_IS_BETTER "  Your leg feels somewhat better.  "

/* forward declare */
class Saiph;

/* includes */
#include <fstream>
#include <list>
#include <map>
#include <string>
#include <vector>
#include "Analyzer.h"
#include "Connection.h"
#include "Globals.h"
#include "Item.h"
#include "Level.h"
#include "Monster.h"
#include "Player.h"
#include "Point.h"
#include "Request.h"
#include "Stash.h"
#include "World.h"
/* analyzers */
#include "Analyzers/Armor.h"
#include "Analyzers/Beatitude.h"
#include "Analyzers/Door.h"
#include "Analyzers/Elbereth.h"
#include "Analyzers/Enhance.h"
#include "Analyzers/Excalibur.h"
#include "Analyzers/Explore.h"
#include "Analyzers/Fight.h"
#include "Analyzers/Food.h"
#include "Analyzers/Health.h"
#include "Analyzers/Loot.h"
#include "Analyzers/MonsterInfo.h"
#include "Analyzers/Potion.h"
#include "Analyzers/Pray.h"
#include "Analyzers/Scroll.h"
#include "Analyzers/Unihorn.h"
#include "Analyzers/Valkyrie.h"
#include "Analyzers/Vault.h"
#include "Analyzers/Wand.h"
#include "Analyzers/Weapon.h"
#include "Analyzers/Wish.h"

/* namespace */
using namespace std;

/* this is our AI */
class Saiph {
	public:
		/* variables */
		World *world;
		ofstream debugfile;
		vector<Level> levels;
		map<unsigned char, Item> inventory;
		map<unsigned char, Item> pickup;
		map<unsigned char, Item> drop;
		Stash *on_ground;
		Coordinate position;
		bool engulfed;
		string last_command;
		bool inventory_changed;
		bool got_pickup_menu;
		bool got_drop_menu;

		/* constructors */
		Saiph(int interface);

		/* destructor */
		~Saiph();

		/* methods */
		bool addItemToInventory(unsigned char key, const Item &item);
		unsigned char directLine(Point point, bool ignore_sinks, bool ignore_boulders);
		const string &farlook(const Point &target);
		Point moveToPoint(unsigned char move);
		bool removeItemFromInventory(unsigned char key, const Item &item);
		bool request(const Request &request);
		bool run();
		unsigned char shortestPath(unsigned char symbol, bool allow_illegal_last_move, int *moves);
		unsigned char shortestPath(const Coordinate &target, bool allow_illegal_last_move, int *moves);
		unsigned char shortestPath(const Point &target, bool allow_illegal_last_move, int *moves);

	private:
		/* variables */
		Connection *connection;
		vector<Analyzer *> analyzers;
		map<string, vector<int> > levelmap; // used for faster map recognition
		bool mines_found;
		bool sokoban_found;
		string farlook_command;
		int last_turn;
		int stuck_counter;

		/* methods */
		void detectPosition();
		bool directLineHelper(const Point &point, bool ignore_sinks, bool ignore_boulders);
		void dumpMaps();
		void parseMessages(const string &messages);
};
#endif
