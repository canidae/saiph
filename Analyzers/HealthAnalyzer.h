#ifndef HEALTHANALYZER_H
#define HEALTHANALYZER_H

/* forward declare */
class HealthAnalyzer;

#include "../Analyzer.h"
#include "../Saiph.h"

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
