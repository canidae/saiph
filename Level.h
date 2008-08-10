#ifndef LEVEL_H
/* defines */
#define LEVEL_H
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
#define LEVEL_ALTAR_HERE "  There is an altar to "
#define LEVEL_FOUNTAIN_DRIES_UP "  The fountain dries up!  "
#define LEVEL_FOUNTAIN_DRIES_UP2 "  As the hand retreats, the fountain disappears!  "
#define LEVEL_FOUNTAIN_HERE "  There is a fountain here.  "
#define LEVEL_OPEN_DOOR_HERE "  There is an open door here.  "
#define LEVEL_STAIRCASE_DOWN_HERE "  There is a staircase down here.  "
#define LEVEL_STAIRCASE_UP_HERE "  There is a staircase up here.  "
#define LEVEL_THERE_IS_NOTHING_HERE "  There is nothing here to pick up.  "
#define LEVEL_THINGS_THAT_ARE_HERE "  Things that are here:  "
#define LEVEL_THINGS_THAT_YOU_FEEL_HERE "  Things that you feel here:  "
#define LEVEL_YOU_FEEL_HERE "  You feel here " // not two spaces here as it's followed by eg. "a lichen corpse"
#define LEVEL_YOU_FEEL_NO_OBJECTS "  You feel no objects here.  "
#define LEVEL_YOU_SEE_HERE "  You see here " // not two spaces here as it's followed by eg. "a lichen corpse"
#define LEVEL_YOU_SEE_NO_OBJECTS "  You see no objects here.  "

/* forward declare */
class Level;

/* includes */
#include <map>
#include <string>
#include "Globals.h"
#include "Item.h"
#include "Monster.h"
#include "Point.h"
#include "Saiph.h"
#include "Stash.h"

/* namespaces */
using namespace std;

/* struct used for pathing */
struct PathNode {
	PathNode *nextnode;
	unsigned int cost;
	unsigned char move;
};

struct PathMap {
	PathNode nodes[MAP_ROW_END + 1][MAP_COL_END + 1];
};

/* Level */
class Level {
	public:
		/* variables */
		PathMap pathmap;
		map<Point, PathMap> pointpathmap;
		unsigned char dungeonmap[MAP_ROW_END + 1][MAP_COL_END + 1];
		unsigned char monstermap[MAP_ROW_END + 1][MAP_COL_END + 1];
		map<Point, Monster> monsters;
		map<Point, Stash> stashes;
		map<unsigned char, map<Point, int> > symbols;
		string name;
		int depth;
		int branch;

		/* static variables */
		static bool passable[UCHAR_MAX + 1];
		static bool track_symbol[UCHAR_MAX + 1];

		/* constructors */
		Level(Saiph *saiph, string name, int branch = BRANCH_MAIN);

		/* methods */
		void parseMessages(const string &messages);
		void setDungeonSymbol(const Point &point, unsigned char symbol, int value = UNKNOWN_SYMBOL_VALUE);
		void setDungeonSymbolValue(const Point &point, int value = UNKNOWN_SYMBOL_VALUE);
		unsigned char shortestPath(const Point &target, bool allow_illegal_last_move, int *moves);
		unsigned char shortestPath(const Point &from, const Point &to, bool allow_illegal_last_move, int *moves);
		void updateMapPoint(const Point &point, unsigned char symbol, int color);
		void updateMonsters();
		void updatePathMap();
		void updatePointPathMap(const Point &point);

	private:
		/* variables */
		Saiph *saiph;
		bool got_drop_menu;
		bool got_pickup_menu;

		/* static variables */
		static Point pathing_queue[PATHING_QUEUE_SIZE];
		static int pathcost[UCHAR_MAX + 1];
		static bool dungeon[UCHAR_MAX + 1];
		static bool monster[UCHAR_MAX + 1];
		static bool item[UCHAR_MAX + 1];
		static bool initialized;

		/* methods */
		void addItemToStash(const Point &point, const Item &item);
		void clearStash(const Point &point);
		unsigned char shortestPath(const PathMap &pathmap, const Point &target, bool allow_illegal_last_move, int *moves);
		void updatePathMap(Point from, PathMap *pathmap);
		bool updatePathMapHelper(const Point &to, const Point &from, PathMap *pathmap);

		/* static methods */
		static void init();
};

#endif
