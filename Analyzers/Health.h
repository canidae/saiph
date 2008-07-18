#ifndef HEALTH_H
/* defines */
#define HEALTH_H
/* messages */
#define HA_ENGRAVE_ADD "Do you want to add to the current engraving?" // should be global
#define HA_ENGRAVE_DUST "What do you want to write in the dust here?" // should be global
#define HA_ENGRAVE_DUST_ADD "What do you want to add to the writing in the dust here?" // should be global
#define HA_ENGRAVE_WITH "What do you want to write with?" // should be global
/* priorities */
#define HEALTH_PRAY_FOR_FOOD 1000
#define HEALTH_ENGRAVE_FOR_HP 900
#define HEALTH_REST_FOR_HP 800

/* forward declare */
class Health;

/* includes */
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* monitors health */
class Health : public Analyzer {
	public:
		/* constructors */
		Health(Saiph *saiph);

		/* methods */
		void command(string *command);
		int finish();
		int parseMessages(string *messages);

	private:
		/* variables */
		Saiph *saiph;
		string action;
		bool resting;
};
#endif
