#ifndef LEVEL_H
#define LEVEL_H
/* pathing */
#define COST_CARDINAL 2
#define COST_DIAGONAL 3
#define COST_FOUNTAIN 4 // better not fight on fountains
#define COST_GRAVE 4 // better not fight on graves
#define COST_ALTAR 4 // better not fight on altars
#define COST_ICE 8 // slippery and risky, try to find a way around (don't try very hard, though)
#define COST_LAVA 512 // lava, hot!
#define COST_MONSTER 64 // try not to path through monsters
#define COST_TRAP 128 // avoid traps
#define COST_WATER 256 // avoid water if possible
#define PATHING_QUEUE_SIZE 16384 // max amount of nodes in pathing_queue
/* max moves a monster can do before we think it's a new monster */
#define MAX_MONSTER_MOVE 3 // if a monster is more than this distance from where we last saw it, then it's probably a new monster
/* debugging that should be moved to Debug in the future */
#define LEVEL_DEBUG_NAME "Level] "
/* messages */
#define LEVEL_ALTAR_HERE "  There is an altar to "
#define LEVEL_FOUNTAIN_DRIES_UP "  The fountain dries up!  "
#define LEVEL_FOUNTAIN_DRIES_UP2 "  As the hand retreats, the fountain disappears!  "
#define LEVEL_FOUNTAIN_HERE "  There is a fountain here.  "
#define LEVEL_GRAVE_HERE "  There is a grave here.  "
#define LEVEL_NO_STAIRS_DOWN_HERE "  You can't go down here.  "
#define LEVEL_NO_STAIRS_UP_HERE "  You can't go up here.  "
#define LEVEL_OPEN_DOOR_HERE "  There is an open door here.  "
#define LEVEL_STAIRCASE_DOWN_HERE "  There is a staircase down here.  "
#define LEVEL_STAIRCASE_UP_HERE "  There is a staircase up here.  "
#define LEVEL_THERE_IS_NOTHING_HERE "  There is nothing here to pick up.  "
#define LEVEL_THINGS_THAT_ARE_HERE "  Things that are here:  "
#define LEVEL_THINGS_THAT_YOU_FEEL_HERE "  Things that you feel here:  "
#define LEVEL_UNDIGGABLE " is too hard to dig in"
#define LEVEL_YOU_FEEL_HERE "  You feel here " // not two spaces here as it's followed by eg. "a lichen corpse"
#define LEVEL_YOU_FEEL_NO_OBJECTS "  You feel no objects here.  "
#define LEVEL_YOU_SEE_HERE "  You see here " // not two spaces here as it's followed by eg. "a lichen corpse"
#define LEVEL_YOU_SEE_NO_OBJECTS "  You see no objects here.  "

#include <limits.h>
#include <map>
#include <string>
#include "Globals.h"
#include "Monster.h"
#include "PathNode.h"
#include "Point.h"
#include "Stash.h"

class Item;
class Saiph;

class Level {
	public:
		PathNode pathmap[MAP_ROW_END + 1][MAP_COL_END + 1];
		unsigned char monstermap[MAP_ROW_END + 1][MAP_COL_END + 1];
		std::map<Point, Monster> monsters;
		std::map<Point, Stash> stashes;
		std::map<Point, int> symbols[UCHAR_MAX + 1];
		std::string name;
		int depth;
		int branch;
		bool undiggable;

		static bool passable[UCHAR_MAX + 1];

		Level(Saiph *saiph, std::string name, int branch = BRANCH_MAIN);

		unsigned char getDungeonSymbol(const Point &point);
		unsigned char getMonsterSymbol(const Point &point);
		void parseMessages(const std::string &messages);
		void setDungeonSymbol(const Point &point, unsigned char symbol);
		const PathNode &shortestPath(const Point &target);
		void updateMapPoint(const Point &point, unsigned char symbol, int color);
		void updateMonsters();
		void updatePathMap();

	private:
		Saiph *saiph;
		PathNode pathnode_outside_map;
		unsigned char dungeonmap[MAP_ROW_END + 1][MAP_COL_END + 1];

		static Point pathing_queue[PATHING_QUEUE_SIZE];
		static unsigned char uniquemap[UCHAR_MAX + 1][CHAR_MAX + 1];
		static int pathcost[UCHAR_MAX + 1];
		static bool dungeon[UCHAR_MAX + 1];
		static bool monster[UCHAR_MAX + 1];
		static bool item[UCHAR_MAX + 1];
		static bool initialized;

		void addItemToStash(const Point &point, const Item &item);
		void clearStash(const Point &point);
		unsigned int updatePathMapHelper(const Point &to, const Point &from);

		static void init();
};

/* inline methods */
inline unsigned char Level::getDungeonSymbol(const Point &point) {
	/* return dungeon symbol at given point */
	if (point.row < MAP_ROW_BEGIN || point.row > MAP_ROW_END || point.col < MAP_COL_BEGIN || point.col > MAP_COL_END)
		return OUTSIDE_MAP;
	return dungeonmap[point.row][point.col];
}

inline unsigned char Level::getMonsterSymbol(const Point &point) {
	/* return monster symbol at given point */
	if (point.row < MAP_ROW_BEGIN || point.row > MAP_ROW_END || point.col < MAP_COL_BEGIN || point.col > MAP_COL_END)
		return ILLEGAL_MONSTER;
	return monstermap[point.row][point.col];
}

inline void Level::setDungeonSymbol(const Point &point, unsigned char symbol) {
	/* need to update both dungeonmap and symbols,
	 * better keep it in a method */
	if (point.row < MAP_ROW_BEGIN || point.row > MAP_ROW_END || point.col < MAP_COL_BEGIN || point.col > MAP_COL_END)
		return; // outside map
	if (dungeonmap[point.row][point.col] == symbol)
		return; // no change
	/* erase old symbol from symbols */
	symbols[dungeonmap[point.row][point.col]].erase(point);
	/* set new symbol in symbols */
	symbols[symbol][point] = UNKNOWN_SYMBOL_VALUE;
	/* update dungeonmap */
	dungeonmap[point.row][point.col] = symbol;
}

inline const PathNode &Level::shortestPath(const Point &point) {
	/* return a PathNode that tells us which direction to move to get to given point */
	if (point.row < MAP_ROW_BEGIN || point.row > MAP_ROW_END || point.col < MAP_COL_BEGIN || point.col > MAP_COL_END)
		return pathnode_outside_map;
	return pathmap[point.row][point.col];
}
#endif
