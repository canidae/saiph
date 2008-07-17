#ifndef FIGHT_H
/* defines */
#define FIGHT_H
/* messages */
#define FIGHT_REALLY_ATTACK "Really attack"
/* priorities */
#define FIGHT_ATTACK_MONSTER 70
#define FIGHT_ATTACK_BLUE_E 2
#define FIGHT_ATTACK_WHITE_AT 1
#define FIGHT_ATTACK_BLUE_AT 1

/* forward declare */
class Fight;

/* includes */
#include <list>
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Monster.h"
#include "../MonsterTracker.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* analyzer for fighting monsters */
class Fight : public Analyzer {
	public:
		/* constructors */
		Fight(Saiph *saiph);

		/* methods */
		void command(string *command);
		int finish();
		int parseMessages(string *messages);

	private:
		/* variables */
		Saiph *saiph;
		string action;
};
#endif
