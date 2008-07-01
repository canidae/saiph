#ifndef SAIPH_H
/* defines */
#define SAIPH_H
/* general defines */
#define MAX_BRANCHES 6
#define MAX_DUNGEON_DEPTH 64
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
#define COST_ICE 8 // slippery and risky, try to find a way around (don't try very hard, though)
#define COST_LAVA 512 // lava, hot!
#define COST_PET 16 // try not to move onto pets
#define COST_TRAP 128 // avoid traps
#define COST_WATER 256 // avoid water if possible
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
#include "Map.h"
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

/* a command to send to the game */
struct Command {
	int analyzer;
	int priority;
};

/* this is our AI */
class Saiph {
	public:
		/* variables */
		Map map[MAX_BRANCHES][MAX_DUNGEON_DEPTH];
		int current_branch;
		int current_level;
		Command command;
		string messages;
		World *world;
		bool engulfed;

		/* constructors */
		Saiph(bool remote);

		/* destructors */
		~Saiph();

		/* methods */
		void farlook(const Point &target);
		void registerAnalyzerSymbols(Analyzer *analyzer, const vector<unsigned char> &symbols);
		bool run();
		unsigned char shortestPath(const Point &target, bool allow_illegal_last_move, int *distance, bool *straight_line);

	private:
		/* variables */
		vector<Analyzer *> analyzers;
		vector<Analyzer *> analyzer_symbols[UCHAR_MAX + 1];
		Connection *connection;
		unsigned int pathmap[ROWS][COLS];
		Point pathing_queue[PATHING_QUEUE_SIZE];
		bool item[UCHAR_MAX + 1];
		bool monster[UCHAR_MAX + 1];
		bool passable[UCHAR_MAX + 1];
		bool static_dungeon_symbol[UCHAR_MAX + 1];

		/* methods */
		void dumpMaps();
		void inspect();
		bool isLegalMove(const Point &to, const Point &from);
		void updateMaps();
		void updatePathMap();
};
#endif
