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
#include "Analyzers/Beatitude.h"
#include "Analyzers/Door.h"
#include "Analyzers/Elbereth.h"
#include "Analyzers/Enhance.h"
#include "Analyzers/Excalibur.h"
#include "Analyzers/Explore.h"
#include "Analyzers/Fight.h"
#include "Analyzers/Food.h"
#include "Analyzers/Health.h"
//#include "Analyzers/Level.h"
#include "Analyzers/Loot.h"
#include "Analyzers/Pray.h"
#include "Analyzers/Wand.h"
#include "Analyzers/Wish.h"
#include "Analyzers/Zorkmid.h"

/* namespace */
using namespace std;

/* struct used for pathing */
struct PathNode {
	PathNode *nextnode;
	unsigned int cost;
	unsigned char move;
};

/* this is our AI */
class Saiph {
	public:
		/* variables */
		World *world;
		ofstream debugfile;
		vector<Level> levels;
		/*
		map<unsigned char, map<int, map<int, map<Point, int> > > > dungeon_feature;
		unsigned char dungeonmap[MAX_BRANCHES][MAX_DUNGEON_DEPTH][MAP_ROW_END + 1][MAP_COL_END + 1];
		unsigned char monstermap[MAX_BRANCHES][MAX_DUNGEON_DEPTH][MAP_ROW_END + 1][MAP_COL_END + 1];
		map<int, map<int, map<Point, Monster> > > monsters;
		map<int, map<int, map<Point, Stash> > > stashes;
		*/
		map<unsigned char, Item> inventory;
		map<unsigned char, Item> pickup;
		map<unsigned char, Item> drop;
		Stash *on_ground;
		Coordinate position;
		bool engulfed;
		int best_priority;

		/* constructors */
		Saiph(int interface);

		/* destructor */
		~Saiph();

		/* methods */
		unsigned char directLine(Point point, bool ignore_sinks);
		void farlook(const Point &target);
		void removeItemFromInventory(unsigned char key, const Item &item);
		void removeItemFromStash(const Point &point, const Item &item);
		bool request(const Request &request);
		bool run();
		void setDungeonSymbolValue(const Point &point, unsigned char symbol, int value);
		unsigned char shortestPath(unsigned char symbol, bool allow_illegal_last_move, int *moves);
		unsigned char shortestPath(const Coordinate &target, bool allow_illegal_last_move, int *moves);
		unsigned char shortestPath(const Point &target, bool allow_illegal_last_move, int *moves);

	private:
		/* variables */
		Connection *connection;
		vector<Analyzer *> analyzers;
		PathNode pathmap[MAP_ROW_END + 1][MAP_COL_END + 1];
		Point pathing_queue[PATHING_QUEUE_SIZE];
		int pathcost[UCHAR_MAX + 1];
		bool passable[UCHAR_MAX + 1];
		bool dungeon[UCHAR_MAX + 1];
		bool monster[UCHAR_MAX + 1];
		bool item[UCHAR_MAX + 1];
		bool track_symbol[UCHAR_MAX + 1];
		int menu;
		map<string, vector<int> > levelmap; // used for faster map recognition

		/* methods */
		void addItemToInventory(unsigned char key, const Item &item);
		void addItemToStash(const Point &point, const Item &item);
		void clearStash(const Point &point);
		void detectPosition();
		bool directLineHelper(const Point &point, bool ignore_sinks);
		unsigned char doPath(const Point &target, bool allow_illegal_last_move, int *moves);
		void dumpMaps();
		map<Point, Monster>::iterator nearestMonster(const Point &point, unsigned char symbol, int color);
		void parseMessages();
		void removeItemFromPickup(const Item &item);
		void setDungeonSymbol(const Point &point, unsigned char symbol);
		void updateMaps();
		void updatePathMap();
		bool updatePathMapHelper(const Point &to, const Point &from);
};
#endif
