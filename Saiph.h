#ifndef SAIPH_H
#define SAIPH_H

#include "Connection.h"
#include "Globals.h"
#include "Player.h"
#include "World.h"

/* general defines */
#define HISTORY 128
#define MAX_DUNGEON 64
#define MAX_EXPLORE 128
#define MAX_SEARCH 50

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

struct Monster {
	int row;
	int col;
	int threat;
	bool eadible;
	bool no_melee;
};

struct Target {
	/* a place we want to explore */
	int row;
	int col;
	int priority;
};

/* this is our AI */
class Saiph {
	public:
		/* constructors */
		Saiph(bool remote);

		/* destructors */
		~Saiph();

		/* methods */
		int main();
		void run();

	private:
		/* variables */
		Connection *connection;
		History history;
		Target target[MAX_EXPLORE];
		int targets;
		World *world;

		/* methods */
		void farlook(int row, int col);
		void inspect();
		void parseMessages();
		void shortestPath();
};

#endif
