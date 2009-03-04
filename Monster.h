#ifndef MONSTER_H
#define MONSTER_H

#include <limits.h>
#include <map>
#include <string>
#include <vector>
#include "Globals.h"

class MonsterData;

class Monster {
	public:
		const MonsterData *data;
		unsigned char symbol;
		int color;
		int last_seen;
		bool visible;
		int attitude;
		bool shopkeeper;
		bool priest;

		Monster(unsigned char symbol = ILLEGAL_MONSTER, int color = 0, int last_seen = -1);
};
#endif
