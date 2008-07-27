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
#define FIGHT_WHITE_AT_PRIORITY ILLEGAL_PRIORITY
#define FIGHT_PICKUP_PRIORITY 425

/* forward declare */
class Fight;

/* includes */
#include <list>
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Monster.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* analyzer for fighting monsters */
class Fight : public Analyzer {
	public:
		/* constructors */
		Fight(Saiph *saiph);

		/* methods */
		void complete();
		void finish();
		void parseMessages(const string &messages);

	private:
		/* variables */
		Saiph *saiph;
		string command2;
		string command3;
		list<string> thrown;
		Request req;
};
#endif
