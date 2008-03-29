#ifndef SAIPH_H
#define SAIPH_H

/* forward declare */
class Saiph;

/* includes */
#include "Analyzer.h"
#include "Connection.h"
#include "Globals.h"
#include "MessageParser.h"
#include "Player.h"
#include "World.h"

/* analyzers */
#include "Analyzers/DoorAnalyzer.h"
#include "Analyzers/ExploreAnalyzer.h"
#include "Analyzers/FightAnalyzer.h"
#include "Analyzers/HealthAnalyzer.h"

/* general defines */
#define HISTORY 128
#define MAX_ANALYZERS 32
#define MAX_BRANCHES 6
#define MAX_BRANCH_DEPTH 64
#define MAX_COMMAND_LENGTH 128
#define MAX_EXPLORE 128
#define MAX_SEARCH 64

/* branches */
#define BRANCH_MAIN 0
#define BRANCH_MINES 1
#define BRANCH_SOKOBAN 2
#define BRANCH_VLAD 3
#define BRANCH_WIZARD 4
#define BRANCH_ASTRAL 5

/* pathing */
#define PATH_MAX_NODES 1680

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

using namespace std;

struct Branch {
	unsigned char door[ROWS][COLS]; // does square have a door?
	unsigned char explore[ROWS][COLS]; // how many times have we been here?
	unsigned char search[ROWS][COLS]; // how many times have we searched here?
};

struct Command {
	char command[MAX_COMMAND_LENGTH];
	int priority;
};

struct History {
	Dungeon map[HISTORY];
	int map_counter;
	int last_pray;
};

/* this is our AI */
class Saiph {
	public:
		/* variables */
		Branch branch[MAX_BRANCHES];
		int current_branch;
		History history;
		MessageParser *parser;
		World *world;

		/* constructors */
		Saiph(bool remote);

		/* destructors */
		~Saiph();

		/* methods */
		void farlook(int row, int col);
		bool isClosedDoor(char symbol);
		bool isMonster(char symbol);
		bool isObject(char symbol);
		bool isOpenDoor(char symbol);
		bool isPassable(char symbol);
		bool isPet(char symbol);
		bool isPlayer(char symbol);
		bool isTrap(char symbol);
		bool isUnexplored(char symbol);
		bool isUnpassable(char symbol);
		void parseMessages();
		bool run();
		void setNextCommand(const char *command, int priority);
		char shortestPath(int row, int col);

	private:
		/* variables */
		Analyzer **analyzers;
		int analyzer_count;
		Connection *connection;
		Command command;

		/* methods */
		void inspect();
};

#endif
