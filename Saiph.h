#ifndef SAIPH_H
#define SAIPH_H

#include "Connection.h"
#include "Globals.h"
#include "Player.h"
#include "World.h"

/* general defines */
#define HISTORY 128
#define MAX_DUNGEON 64

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

struct Monster {
	int row;
	int col;
	int threat;
	bool eadible;
	bool no_melee;
};

struct History {
	Dungeon map[HISTORY];
	int map_counter;
	Dungeon search[MAX_DUNGEON]; // FIXME: branches (mines/astral/sokoban/vlad/wizard)
	int last_pray;
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
		World *world;

		/* methods */
		void farlook(int row, int col);
		void inspect();
		void parseMessages();
};

#endif
