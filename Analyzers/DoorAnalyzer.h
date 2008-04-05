#ifndef DOORANALYZER_H
/* defines */
#define DOORANALYZER_H
/* various */
#define DO_MAX_DOORS 16
#define DO_UNLOCK_DOOR 30
#define DO_PICK_DOOR 20
#define DO_KICK_DOOR 50 // temporary hack

/* forward declare */
class DoorAnalyzer;

/* includes */
#include "../Saiph.h"

/* namespace */
using namespace std;

/* analyzes the map and finds somewhere to explore */
class DoorAnalyzer : public Analyzer {
	public:
		/* constructors */
		DoorAnalyzer(Saiph *saiph);

		/* methods */
		int finish();
		void command();

	private:
		/* variables */
		Saiph *saiph;
};
#endif
