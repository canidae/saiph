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
		unsigned char branch;

		/* static variables */
		static bool passable[UCHAR_MAX + 1];
		static bool track_symbol[UCHAR_MAX + 1];

		/* constructors */
		Level(Saiph *saiph, string name, unsigned char branch = BRANCH_MAIN);

		/* methods */
		void parseMessages(const string &messages);
		void setDungeonSymbol(const Point &point, unsigned char symbol, int value = -1);
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
