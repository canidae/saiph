#ifndef HEALTHANALYZER_H
/* defines */
#define HEALTHANALYZER_H
/* various */
#define HA_ENGRAVE_ELBERETH "E-y Elbereth\n"
#define HA_ENGRAVE_ELBERETH_FRESH "E-n Elbereth\n"
#define HA_PRAY "#pray\n"

/* forward declare */
class HealthAnalyzer;

/* includes */
#include "../Saiph.h"

/* namespace */
using namespace std;

/* monitors health */
class HealthAnalyzer : public Analyzer {
	public:
		/* variables */
		Saiph *saiph;

		/* constructors */
		HealthAnalyzer(Saiph *saiph);

		/* methods */
		void finish();
};
#endif
