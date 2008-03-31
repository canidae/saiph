#ifndef LEVELANALYZER_H
#define LEVELANALYZER_H

/* forward declare */
class LevelAnalyzer;

/* includes */
#include "../Analyzer.h"
#include "../Saiph.h"

/* defines */
#define LA_DESCEND_PRIORITY 9

using namespace std;

class LevelAnalyzer : public Analyzer {
	public:
		/* constructors */
		LevelAnalyzer(Saiph *saiph);

		/* destructors */
		~LevelAnalyzer();

		/* methods */
		virtual void end();

	private:
		/* variables */
		Saiph *saiph;
};

#endif
