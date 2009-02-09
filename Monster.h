#ifndef MONSTER_H
#define MONSTER_H

#include "Globals.h"

class Monster {
	public:
		unsigned char symbol;
		int color;
		int last_seen;
		bool visible;
		int attitude;
		bool shopkeeper;
		bool priest;
		bool minotaur;

		Monster(unsigned char symbol = ILLEGAL_MONSTER, int color = 0, int last_seen = -1, bool visible = false, int attitude = ATTITUDE_UNKNOWN, bool shopkeeper = false, bool priest = false, bool minotaur = false);
};
#endif
