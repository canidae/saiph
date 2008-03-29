#ifndef EXPLOREANALYZER_H
#define EXPLOREANALYZER_H

/* forward declare */
class ExploreAnalyzer;

/* includes */
#include "../Analyzer.h"
#include "../Saiph.h"

/* defines */
#define EA_MAX_PLACES 128

using namespace std;

struct Place {
	int row;
	int col;
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
		Place places[EA_MAX_PLACES];
		int place_count;
		int max_score;
		Saiph *saiph;
};

#endif
