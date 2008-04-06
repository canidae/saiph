#ifndef HEALTHANALYZER_H
/* defines */
#define HEALTHANALYZER_H
/* messages */
#define HA_ENGRAVE_WITH "What do you want to write with?" // should be global
#define HA_ENGRAVE_DUST "What do you want to write in the dust here?" // should be global
/* actions */
#define HA_NOTHING 0
#define HA_ENGRAVE 1
#define HA_ENGRAVE_ELBERETH 2
#define HA_PRAY 3
#define HA_USE_HANDS 4

/* forward declare */
class HealthAnalyzer;

/* includes */
#include "../Saiph.h"

/* namespace */
using namespace std;

/* monitors health */
class HealthAnalyzer : public Analyzer {
	public:
		/* constructors */
		HealthAnalyzer(Saiph *saiph);

		/* methods */
		int parseMessages(string *messages);
		int finish();
		void command();

	private:
		/* variables */
		Saiph *saiph;
		int action;
};
#endif
