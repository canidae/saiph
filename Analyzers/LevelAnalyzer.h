#ifndef LEVELANALYZER_H
/* defines */
#define LEVELANALYZER_H
/* various */
#define LA_DESCEND_PRIORITY 9

/* forward declare */
class LevelAnalyzer;

/* includes */
#include "../Saiph.h"

/* namespace */
using namespace std;

/* class for ascending/descending */
class LevelAnalyzer : public Analyzer {
	public:
		/* constructors */
		LevelAnalyzer(Saiph *saiph);

		/* methods */
		int finish();
		void command();

	private:
		/* variables */
		Saiph *saiph;
		unsigned char action;
};
#endif
