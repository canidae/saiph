#ifndef MONSTER_H
/* defines */
#define MONSTER_H

/* forward declare */
class Monster;

/* includes */
#include "Globals.h"

/* namespaces */
using namespace std;

/* Monster */
class Monster {
	public:
		/* variables */
		unsigned char symbol;
		int color;
		bool visible;

		/* constructors */
		Monster(unsigned char symbol = ILLEGAL_MONSTER, int color = 0, bool visible = false);
};
#endif
