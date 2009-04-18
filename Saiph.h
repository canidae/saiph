#ifndef SAIPH_H
#define SAIPH_H
/* debug */
#define COMMAND_DEBUG_NAME "Command] "
#define ITEMTRACKER_DEBUG_NAME "ItemTracker] "
#define MESSAGES_DEBUG_NAME "Messages] "
#define SAIPH_DEBUG_NAME "Saiph] "
/* max length of levelname */
#define MAX_LEVELNAME_LENGTH 16
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
#define SAIPH_POLYMORPH "  You turn into "
#define SAIPH_UNPOLYMORPH "  You return to "
#define SAIPH_BEGIN_LEVITATION "  You start to float in the air!  "
#define SAIPH_BEGIN_LEVITATION_PIT "  You float up, out of the pit!  "
#define SAIPH_END_LEVITATION "  You float gently to the " //followed by kind of floor
#define SAIPH_END_LEVITATION_SINK "  You crash to the " //followed by kind of floor

#include <fstream>
#include <map>
#include <string>
#include <vector>
#include "Coordinate.h"
#include "Item.h"
#include "Level.h"
#include "Point.h"

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
	static bool lycanthropy;
	static bool hurt_leg;
	static bool polymorphed;
	static bool levitating;
	static bool engulfed;
	/* position */
	static Coordinate position;
	/* zorkmids */
	static int zorkmids;
	/* intrinsics/extrinsics */
	static unsigned long long int intrinsics;
	static unsigned long long int extrinsics;

	std::map<unsigned char, Item> pickup; // Loot analyzer
	std::map<unsigned char, Item> drop; // Loot analyzer
	std::string last_command; // World?
	int internal_turn; // World
	bool got_pickup_menu; // Loot analyzer
	bool got_drop_menu; // Loot analyzer
	int last_turn; // World?

	Saiph();
	~Saiph();

	static void init();
	static void destroy();
	static void analyze();
	static void parseMessages(const std::string &messages);

	unsigned char directLine(Point point, bool ignore_sinks, bool ignore_boulders); // World
	bool run(); // main?

private:
	std::vector<analyzer::Analyzer *>::iterator best_analyzer; // main?
	int stuck_counter; // main?

	bool directLineHelper(const Point &point, bool ignore_sinks, bool ignore_boulders); // World
	Point directionToPoint(unsigned char direction); // World
};
#endif
