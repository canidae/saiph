#ifndef SAIPH_H
/* defines */
#define SAIPH_H
/* general defines */
#define HISTORY 128
#define MAX_ANALYZERS 32
#define MAX_BRANCHES 6
#define MAX_BRANCH_DEPTH 64
#define MAX_COMMAND_LENGTH 128
#define MAX_DUNGEON_DEPTH 64
#define MAX_EXPLORE 128
#define MAX_SEARCH 16
/* branches */
#define BRANCH_MAIN 0
#define BRANCH_MINES 1
#define BRANCH_SOKOBAN 2
#define BRANCH_VLAD 3
#define BRANCH_WIZARD 4
#define BRANCH_ASTRAL 5
/* pathing */
#define COST_CARDINAL 2
#define COST_DIAGONAL 3
#define COST_TRAP 128 // try hard to avoid traps
#define MAX_NODES 512 // should cover even the most wicked dungeons
/* questions looks like this
 * ? [ynq] (n)
 */
/* movement */
#define MOVE_NW 'y'
#define MOVE_N 'k'
#define MOVE_NE 'u'
#define MOVE_W 'h'
#define MOVE_E 'l'
#define MOVE_SW 'b'
#define MOVE_S 'j'
#define MOVE_SE 'n'
#define MOVE_UP '<'
#define MOVE_DOWN '>'

/* forward declare */
class Saiph;

/* includes */
#include "Analyzer.h"
#include "Connection.h"
#include "MessageParser.h"
#include "Player.h"
#include "World.h"
/* analyzers */
#include "Analyzers/DungeonAnalyzer.h"
#include "Analyzers/ExploreAnalyzer.h"
#include "Analyzers/FightAnalyzer.h"
#include "Analyzers/HealthAnalyzer.h"

/* namespace */
using namespace std;

/* a branch holds a "static map" for all branches/dlvl's */
struct Branch {
	char map[MAX_DUNGEON_DEPTH][ROWS][COLS]; // map of dungeon minus dynamic stuff (monsters, objects, etc)
	char search[MAX_DUNGEON_DEPTH][ROWS][COLS]; // how many times have we searched here?
};

/* a command to send to the game */
struct Command {
	char command[MAX_COMMAND_LENGTH];
	int priority;
};

/* TODO */
struct History {
	Dungeon map[HISTORY];
	int map_counter;
	int last_pray;
};

/* this is our AI */
class Saiph {
	public:
		/* variables */
		Branch branches[MAX_BRANCHES];
		int current_branch;
		History history;
		MessageParser *parser;
		World *world;

		/* constructors */
		Saiph(bool remote);

		/* destructors */
		~Saiph();

		/* methods */
		void dumpScreens();
		void farlook(int row, int col);
		char findNextDirection(const int to_row, const int to_col, int &from_row, int &from_col);
		bool hasBoulder(int branch, int dungeon, int row, int col);
		bool hasClosedDoor(int branch, int dungeon, int row, int col);
		bool hasCorridor(int branch, int dungeon, int row, int col);
		bool hasMonster(int branch, int dungeon, int row, int col);
		bool hasObject(int branch, int dungeon, int row, int col);
		bool hasOpenDoor(int branch, int dungeon, int row, int col);
		bool hasPassable(int branch, int dungeon, int row, int col);
		bool hasPet(int branch, int dungeon, int row, int col);
		bool hasPlayer(int branch, int dungeon, int row, int col);
		bool hasTrap(int branch, int dungeon, int row, int col);
		bool hasUnexplored(int branch, int dungeon, int row, int col);
		bool hasUnpassable(int branch, int dungeon, int row, int col);
		bool isBoulder(char symbol);
		bool isClosedDoor(char symbol);
		bool isCorridor(char symbol);
		bool isMonster(char symbol);
		bool isObject(char symbol);
		bool isOpenDoor(char symbol);
		bool isPassable(char symbol);
		bool isPet(char symbol);
		bool isPlayer(char symbol);
		bool isTrap(char symbol);
		bool isUnexplored(char symbol);
		bool isUnpassable(char symbol);
		bool run();
		void setNextCommand(const char *command, int priority);
		char shortestPath(int branch, int dungeon, int row, int col, int &distance, bool &direct_line);

	private:
		/* variables */
		Analyzer **analyzers;
		int analyzer_count;
		Command command;
		Connection *connection;
		unsigned short pathcost[ROWS][COLS];
		unsigned char pathpos[MAX_NODES][2];

		/* methods */
		void inspect();
};

#endif
