#ifndef EXPLOREANALYZER_H
#define EXPLOREANALYZER_H

/* forward declare */
class ExploreAnalyzer;

/* includes */
#include "../Analyzer.h"
#include "../Saiph.h"

/* defines */
#define EA_MAX_PLACES 128
#define EA_DEAD_END_VALUE 40
#define EA_TURN_VALUE 30
#define EA_STRAIGHT_PATH_VALUE 20
#define EA_T_CROSS_VALUE 10
#define EA_NOTHING_VALUE 0

using namespace std;

struct EA_Place {
	int row;
	int col;
	char move;
	int value;
};

class ExploreAnalyzer : public Analyzer {
	public:
		/* constructors */
		ExploreAnalyzer(Saiph *saiph);

		/* destructors */
		~ExploreAnalyzer();

		/* methods */
		virtual void analyze(int row, int col, char symbol);
		virtual void finish();

	private:
		/* variables */
		EA_Place places[EA_MAX_PLACES];
		int place_count;
		Saiph *saiph;
};

#endif
