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
#include "Analyzers/HealthAnalyzer.h"
#include "Analyzers/FightAnalyzer.h"

/* general defines */
#define HISTORY 128
#define MAX_ANALYZERS 16
#define MAX_COMMAND_LENGTH 128
#define MAX_DUNGEON 64
#define MAX_EXPLORE 128
#define MAX_SEARCH 64

/* pathing */
#define PATH_MAX_NODES 1680

/* enemies */
#define EADIBLE_CLASSES "beghjmopqtuwxzCDGHJNORTUWXY:"
#define MAX_ENEMIES 128

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

struct History {
	Dungeon map[HISTORY];
	int map_counter;
	char search[MAX_DUNGEON][ROWS][COLS]; // FIXME: branches (mines/astral/sokoban/vlad/wizard)
	int last_pray;
};

struct Command {
	char command[MAX_COMMAND_LENGTH];
	int priority;
};

/* this is our AI */
class Saiph {
	public:
		/* variables */
		History history;
		MessageParser *parser;
		World *world;

		/* constructors */
		Saiph(bool remote);

		/* destructors */
		~Saiph();

		/* methods */
		void farlook(int row, int col);
		void parseMessages();
		void run();
		void setNextCommand(const char *command, int priority);
		char shortestPath(int row, int col);

	private:
		/* variables */
		Analyzer **analyzers;
		int analyzer_count;
		Connection *connection;
		Command command;

		/* methods */
		bool isMonster(char symbol);
		bool isPassable(char symbol);
		void inspect();
};

#endif
