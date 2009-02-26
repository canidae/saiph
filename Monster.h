#ifndef MONSTER_H
#define MONSTER_H

#include <limits.h>
#include <map>
#include <string>
#include <vector>
#include "Globals.h"

class Monster {
	public:
		int id;
		unsigned char symbol;
		int color;
		int last_seen;
		bool visible;
		int attitude;
		bool shopkeeper;
		bool priest;
		bool minotaur;

		Monster(unsigned char symbol = ILLEGAL_MONSTER, int color = 0, int last_seen = -1);
};
#endif
