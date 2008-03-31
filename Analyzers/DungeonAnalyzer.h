#ifndef DUNGEONANALYZER_H
/* defines */
#define DUNGEONANALYZER_H

/* forward declare */
class DungeonAnalyzer;

/* includes */
#include "../Saiph.h"

/* namespace */
using namespace std;

/* analyzes the dungeon layout */
class DungeonAnalyzer : public Analyzer {
	public:
		/* constructors */
		DungeonAnalyzer(Saiph *saiph);

		/* methods */
		virtual void start();

	private:
		/* variables */
		Saiph *saiph;
};
#endif
