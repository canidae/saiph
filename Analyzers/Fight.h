#ifndef FIGHT_H
/* defines */
#define FIGHT_H
/* messages */
#define FIGHT_REALLY_ATTACK "Really attack"
/* priorities */
#define FIGHT_MOVE_PRIORITY 325
#define FIGHT_ATTACK_PRIORITY 450
#define FIGHT_BLUE_E_PRIORITY 30
#define FIGHT_BLUE_AT_PRIORITY ILLEGAL_PRIORITY
#define FIGHT_LOOT_PRIORITY 350

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
		void finish();
		void parseMessages(const string &messages);

	private:
		/* variables */
		Saiph *saiph;
		string action;
		string action2;
		string action3;
		list<string> thrown;
		Request req;
};
#endif
