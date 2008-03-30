#ifndef DUNGEONANALYZER_H
#define DUNGEONANALYZER_H

/* forward declare */
class DungeonAnalyzer;

/* includes */
#include "../Analyzer.h"
#include "../Saiph.h"

/* defines */
#define DA_NO_DUNGEON 0
#define DA_DUNGEON_OPEN 1
#define DA_DUNGEON_CLOSED 2

using namespace std;

class DungeonAnalyzer : public Analyzer {
	public:
		/* constructors */
		DungeonAnalyzer(Saiph *saiph);

		/* destructors */
		~DungeonAnalyzer();

		/* methods */
		virtual void analyze(int row, int col, char symbol);
		virtual void finish();

	private:
		/* variables */
		Saiph *saiph;
};

#endif
