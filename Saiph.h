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
class Connection;
class Stash;
class World;

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
	/* position, level name and turn */
	static Coordinate position;
	static char levelname[MAX_LEVELNAME_LENGTH];
	static int turn;
	/* zorkmids */
	static int zorkmids;
	/* intrinsics/extrinsics */
	static unsigned long long int intrinsics;
	static unsigned long long int extrinsics;

	std::vector<Level> levels;
	std::map<unsigned char, Item> inventory;
	std::map<unsigned char, Item> pickup;
	std::map<unsigned char, Item> drop;
	Stash *on_ground;
	Coordinate branch_main;
	Coordinate branch_mines;
	Coordinate branch_sokoban;
	std::string last_command;
	int internal_turn;
	bool inventory_changed;
	bool got_pickup_menu;
	bool got_drop_menu;
	int last_turn;

	Saiph();
	~Saiph();

	bool addItemToInventory(unsigned char key, const Item &item);
	unsigned char directLine(Point point, bool ignore_sinks, bool ignore_boulders);
	const std::string &farlook(const Point &target);
	unsigned char getDungeonSymbol();
	unsigned char getDungeonSymbol(const Coordinate &coordinate);
	unsigned char getDungeonSymbol(const Point &point);
	unsigned char getDungeonSymbol(unsigned char direction);
	unsigned char getMonsterSymbol(const Coordinate &coordinate);
	unsigned char getMonsterSymbol(const Point &point);
	bool removeItemFromInventory(unsigned char key, const Item &item);
	bool run();
	void setDungeonSymbol(const Coordinate &coordinate, unsigned char symbol);
	void setDungeonSymbol(const Point &point, unsigned char symbol);
	void setDungeonSymbol(unsigned char symbol);
	PathNode shortestPath(const Coordinate &target);
	const PathNode &shortestPath(const Point &target);
	PathNode shortestPath(unsigned char symbol);

private:
	Connection *connection;
	World *world;
	std::vector<analyzer::Analyzer *> analyzers;
	std::map<std::string, std::vector<int> > levelmap; // used for faster map recognition
	std::vector<analyzer::Analyzer *>::iterator best_analyzer;
	std::string farlook_command;
	bool sokoban_found;
	int stuck_counter;
	time_t start_time;

	void detectPosition();
	bool directLineHelper(const Point &point, bool ignore_sinks, bool ignore_boulders);
	Point directionToPoint(unsigned char direction);
	void dumpMaps();
	void parseMessages(const std::string &messages);
};

/* inline methods */
inline unsigned char Saiph::getDungeonSymbol() {
	/* return dungeon symbol at player position */
	return levels[position.level].getDungeonSymbol(position);
}

inline unsigned char Saiph::getDungeonSymbol(const Coordinate &coordinate) {
	/* return dungeon symbol at given coordinate */
	if (coordinate.level < 0 || coordinate.level > (int) levels.size())
		return OUTSIDE_MAP;
	return levels[coordinate.level].getDungeonSymbol(coordinate);
}

inline unsigned char Saiph::getDungeonSymbol(const Point &point) {
	/* return dungeon symbol at given point on current level */
	return levels[position.level].getDungeonSymbol(point);
}

inline unsigned char Saiph::getDungeonSymbol(unsigned char direction) {
	/* return dungeon symbol in given direction on current level */
	switch (direction) {
		case NW:
			return getDungeonSymbol(Point(position.row - 1, position.col - 1));

		case N:
			return getDungeonSymbol(Point(position.row - 1, position.col));

		case NE:
			return getDungeonSymbol(Point(position.row - 1, position.col + 1));

		case W:
			return getDungeonSymbol(Point(position.row, position.col - 1));

		case NOWHERE:
		case DOWN:
		case UP:
			return getDungeonSymbol();

		case E:
			return getDungeonSymbol(Point(position.row, position.col + 1));

		case SW:
			return getDungeonSymbol(Point(position.row + 1, position.col - 1));

		case S:
			return getDungeonSymbol(Point(position.row + 1, position.col));

		case SE:
			return getDungeonSymbol(Point(position.row + 1, position.col + 1));

		default:
			return OUTSIDE_MAP;
	}
}

inline unsigned char Saiph::getMonsterSymbol(const Coordinate &coordinate) {
	/* return monster symbol at given point on current level */
	if (coordinate.level < 0 || coordinate.level > (int) levels.size())
		return ILLEGAL_MONSTER;
	return levels[coordinate.level].getMonsterSymbol(coordinate);
}

inline unsigned char Saiph::getMonsterSymbol(const Point &point) {
	/* return monster symbol at given point on current level */
	return levels[position.level].getMonsterSymbol(point);
}

inline void Saiph::setDungeonSymbol(unsigned char symbol) {
	/* set dungeon symbol at player position */
	levels[position.level].setDungeonSymbol(position, symbol);
}

inline void Saiph::setDungeonSymbol(const Coordinate &coordinate, unsigned char symbol) {
	/* set dungeon symbol at given coordinate */
	if (coordinate.level < 0 || coordinate.level > (int) levels.size())
		return;
	levels[coordinate.level].setDungeonSymbol(coordinate, symbol);
}

inline void Saiph::setDungeonSymbol(const Point &point, unsigned char symbol) {
	/* set dungeon symbol at given point on current level */
	levels[position.level].setDungeonSymbol(point, symbol);
}

inline const PathNode &Saiph::shortestPath(const Point &point) {
	/* returns PathNode for shortest path from player to target */
	return levels[position.level].shortestPath(point);
}

#endif
