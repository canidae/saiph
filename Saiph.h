#ifndef SAIPH_H
/* defines */
#define SAIPH_H
/* general defines */
#define MAX_BRANCHES 6
#define MAX_DUNGEON_DEPTH 64
#define MAX_HISTORY 128
#define MAX_SEARCH 10 // triple the value in dead end corridors
/* branches */
#define BRANCH_MAIN 0
#define BRANCH_MINES 1
#define BRANCH_SOKOBAN 2
#define BRANCH_VLAD 3
#define BRANCH_WIZARD 4
#define BRANCH_ASTRAL 5
/* pathing */
#define NOT_BLOCKED 0
#define BLOCKED 1
#define TEMPORARY_BLOCKED 2
#define COST_CARDINAL 2
#define COST_DIAGONAL 3
#define COST_LAVA 16384 // lava, hot!
#define COST_PET 16 // try not to move onto pets
#define COST_TRAP 128 // avoid traps
#define COST_WATER 4096 // avoid water if possible
#define PATHING_QUEUE_SIZE 4096 // max amount of nodes in pathing_queue
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
#define ILLEGAL_MOVE 0
/* actions */
#define ENGRAVE 'E'
#define KICK 4 // ctrl-d
#define OPEN 'o'
#define PRAY "#pray\n"
#define REST '.'
#define SEARCH 's'
/* elbereth */
#define ELBERETH "Elbereth\n"
/* various */
#define HANDS '-'
#define NO 'n'
#define YES 'y'

/* forward declare */
class Saiph;

/* includes */
#include <list>
#include <string>
#include <vector>
#include "Analyzer.h"
#include "Connection.h"
#include "Dungeon.h"
#include "Player.h"
#include "Point.h"
#include "World.h"
/* analyzers */
#include "Analyzers/DoorAnalyzer.h"
#include "Analyzers/FoodAnalyzer.h"
#include "Analyzers/ExploreAnalyzer.h"
#include "Analyzers/HealthAnalyzer.h"
#include "Analyzers/LevelAnalyzer.h"
#include "Analyzers/LootAnalyzer.h"
#include "Analyzers/MonsterAnalyzer.h"

/* namespace */
using namespace std;

/* a branch holds numerous "maps" for all dungeons in the given (visited) branch */
struct Branch {
	unsigned char map[MAX_DUNGEON_DEPTH][ROWS][COLS]; // map of dungeon minus dynamic stuff (no monsters, objects, etc)
	unsigned char search[MAX_DUNGEON_DEPTH][ROWS][COLS]; // how many times have we searched here?
	unsigned char unpassable[MAX_DUNGEON_DEPTH][ROWS][COLS]; // unpassable tiles
	unsigned char diagonally_unpassable[MAX_DUNGEON_DEPTH][ROWS][COLS]; // tiles we can't diagonally pass
};

/* a command to send to the game */
struct Command {
	int analyzer;
	int priority;
};

/* this is our AI */
class Saiph {
	public:
		/* variables */
		Branch **branches;
		int current_branch;
		Command command;
		string messages;
		World *world;
		bool engulfed;
		list<Dungeon> *history;

		/* constructors */
		Saiph(bool remote);

		/* destructors */
		~Saiph();

		/* methods */
		Point directionToPos(unsigned char direction, Point target);
		void dumpMaps();
		void farlook(const Point &target);
		bool isLegalMove(const Point &to, const Point &from);
		unsigned char moveToDirection(const Point &to, const Point &from);
		void registerAnalyzerSymbols(Analyzer *analyzer, const vector<unsigned char> &symbols);
		bool run();
		unsigned char shortestPath(const Point &target, bool allow_illegal_last_move, int *distance, bool *straight_line);

	private:
		/* variables */
		vector<Analyzer *> analyzers;
		vector<Analyzer *> analyzer_symbols[UCHAR_MAX + 1];
		Connection *connection;
		bool ismonster[UCHAR_MAX + 1];
		unsigned int pathcost[ROWS][COLS];
		Point pathing_queue[PATHING_QUEUE_SIZE];
		bool passable[UCHAR_MAX + 1];
		bool diagonally_passable[UCHAR_MAX + 1];
		bool static_dungeon_symbol[UCHAR_MAX + 1];

		/* methods */
		void inspect();
		void updateMaps();
		void updatePathMap();
};
#endif
