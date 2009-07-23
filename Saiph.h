#ifndef SAIPH_H
#define SAIPH_H
/* debug */
#define SAIPH_DEBUG_NAME "Saiph] "

#include <map>
#include <string>
#include "Coordinate.h"
#include "Item.h"

namespace analyzer {
	class Analyzer;
}

class Saiph {
public:
	/* attributes */
	static int alignment;
	static int charisma;
	static int constitution;
	static int dexterity;
	static int intelligence;
	static int strength;
	static int wisdom;
	/* status */
	static int armor_class;
	static int encumbrance;
	static int experience;
	static int hunger;
	static int hitpoints;
	static int hitpoints_max;
	static int power;
	static int power_max;
	/* effects */
	static bool blind;
	static bool confused;
	static bool foodpoisoned;
	static bool hallucinating;
	static bool ill;
	static bool slimed;
	static bool stunned;
	static bool hurt_leg;
	static bool polymorphed;
	/* position */
	static Coordinate position;
	/* zorkmids */
	static int zorkmids;
	/* intrinsics/extrinsics */
	static unsigned long long int intrinsics;
	static unsigned long long int extrinsics;
	/* last pray turn */
	static int last_pray_turn;

	static void analyze();
	static void parseMessages(const std::string &messages);
};
#endif
