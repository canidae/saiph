#ifndef FIGHT_H
/* defines */
#define FIGHT_H
/* messages */
#define FIGHT_REALLY_ATTACK "Really attack"
/* priorities */
#define FIGHT_CONTINUE_ACTION 1000
#define FIGHT_ATTACK_MONSTER 70

/* forward declare */
class Fight;

/* includes */
#include <string>
#include <vector>
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
		void inspect(const Point &point, unsigned char symbol);
		int parseMessages(string *messages);

	private:
		/* variables */
		Saiph *saiph;
		vector<unsigned char> monsterlist[MAX_BRANCHES][MAX_DUNGEON_DEPTH];
		string action;
};
#endif
