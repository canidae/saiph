#ifndef MONSTER_H
/* defines */
#define MONSTER_H

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

		/* constructors */
		Monster(unsigned char symbol = ILLEGAL_MONSTER, int color = 0);
};
#endif
