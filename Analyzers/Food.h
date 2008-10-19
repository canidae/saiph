#ifndef FOOD_H
#define FOOD_H
/* some text */
#define FOOD_CORPSE " corpse"
#define FOOD_CORPSES " corpses"
#define FOOD_EAT_IT_1 "  There is "
#define FOOD_EAT_IT_2 " here; eat it? "
#define FOOD_EAT_ONE_1 "  There are "
#define FOOD_EAT_ONE_2 " here; eat one? "
#define FOOD_IS_DESTROYED " is destroyed!  "
#define FOOD_IS_KILLED " is killed!  "
#define FOOD_STOP_EATING "  Stop eating? [yn] (y)  "
#define FOOD_YOU_DESTROY "  You destroy the "
#define FOOD_YOU_KILL "  You kill the "
/* how old corpses may be */
#define FOOD_CORPSE_EAT_TIME 30
/* priorities */
#define FOOD_EAT_CORPSE_PRIORITY 435
#define FOOD_PICKUP_PRIORITY 425
#define FOOD_EAT_HUNGRY_PRIORITY 350
#define FOOD_EAT_WEAK_PRIORITY 725
#define FOOD_EAT_FAINTING_PRIORITY 750
#define FOOD_PRAY_FOR_FOOD 950
/* effects we should avoid unless we're protected */
#define FOOD_PICKUP_INSTEAD 0x00001 // used for lichen & lizard corpses
#define FOOD_ACIDIC 0x00002
#define FOOD_AGGRAVATE 0x00004
#define FOOD_DIE 0x00008
#define FOOD_DWARF 0x00010
#define FOOD_ELF 0x00020
#define FOOD_GNOME 0x00040
#define FOOD_HALLUCINOGENIC 0x00080
#define FOOD_HUMAN 0x00100
#define FOOD_LYCANTHROPY 0x00200
#define FOOD_MIMIC 0x00400
#define FOOD_PETRIFY 0x00800
#define FOOD_POISONOUS 0x01000
#define FOOD_POLYMORPH 0x02000
#define FOOD_SLIME 0x04000
#define FOOD_STUN 0x08000
#define FOOD_TELEPORTITIS 0x10000
#define FOOD_VEGAN 0x20000
#define FOOD_VEGETARIAN 0x40000

#include <map>
#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Request.h"

class Saiph;

class Food : public Analyzer {
	public:
		Food(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);
		bool request(const Request &request);

	private:
		Saiph *saiph;
		std::string command2;
		std::vector<std::string> eat_order;
		std::map<std::string, int> corpse_data;
		std::map<Point, unsigned char> prev_monster_loc;
		std::map<Point, int> safe_eat_loc;
		Request req;

		bool safeToEat(const std::string &corpse);
};
#endif
