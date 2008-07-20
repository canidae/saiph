#ifndef MONSTERTRACKER_H
/* defines */
#define MONSTERTRACKER_H

/* forward declare */
class MonsterTracker;

/* includes */
#include <list>
#include <map>
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
		bool monster[UCHAR_MAX + 1];
		map<int, map<int, map<Point, Monster> > > monsters;

		/* constructors */
		MonsterTracker(Saiph *saiph);

		/* methods */
		void begin();
		void removeMonsters();
		void updateMonster(const Point &point);

	private:
		/* variables */
		Saiph *saiph;
};
#endif
