#ifndef MONSTERTRACKER_H
/* defines */
#define MONSTERTRACKER_H

/* forward declare */
class MonsterTracker;

/* includes */
#include <list>
#include "Globals.h"
#include "Monster.h"
#include "Point.h"
#include "Saiph.h"

/* namespaces */
using namespace std;

/* MonsterTracker */
class MonsterTracker {
	public:
		/* variables */
		list<Monster> monsters[MAX_BRANCHES][MAX_DUNGEON_DEPTH];

		/* constructors */
		MonsterTracker(Saiph *saiph);

		/* methods */
		void removeMonsters();
		void updateMonster(const Point &point);

	private:
		/* variables */
		Saiph *saiph;
};
#endif
