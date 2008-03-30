#ifndef HEALTHANALYZER_H
#define HEALTHANALYZER_H

/* forward declare */
class HealthAnalyzer;

/* includes */
#include "../Analyzer.h"
#include "../Saiph.h"

/* defines */
#define HA_ENGRAVE_ELBERETH "E-y Elbereth\n"
#define HA_ENGRAVE_ELBERETH_FRESH "E-n Elbereth\n"
#define HA_PRAY "#pray\n"

using namespace std;

class HealthAnalyzer : public Analyzer {
	public:
		/* variables */
		Saiph *saiph;

		/* constructors */
		HealthAnalyzer(Saiph *saiph);

		/* destructors */
		~HealthAnalyzer();

		/* methods */
		void finish();
};

#endif
