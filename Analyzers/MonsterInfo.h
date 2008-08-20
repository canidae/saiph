#ifndef MONSTERINFO_H
/* defines */
#define MONSTERINFO_H

/* forward declare */
class MonsterInfo;

/* includes */
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Monster.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* analyzer that fetch additional info for certain monsters (@, A, aligned unicorn, minotaur, riders) */
class MonsterInfo : public Analyzer {
	public:
		/* constructors */
		MonsterInfo(Saiph *saiph);

		/* methods */
		void analyze();
		void parseMessages(const string &messages);

	private:
		/* variables */
		Saiph *saiph;
		map<Point, Monster>::iterator look_at;
		bool checking;
};
#endif
