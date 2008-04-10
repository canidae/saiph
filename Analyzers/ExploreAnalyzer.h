#ifndef EXPLOREANALYZER_H
/* defines */
#define EXPLOREANALYZER_H
/* various */
#define EX_MAX_PLACES 1024
#define EX_UNLIT_ROOM 20 // room where we haven't seen the floor
#define EX_DEAD_END 15 // corridor that just ends
#define EX_TURN 10 // corridor that turns
#define EX_EXTRA_SEARCH 5 // frantic searching for door
#define EX_DEAD_END_MULTIPLIER 3 // search more frantically in dead ends

/* forward declare */
class ExploreAnalyzer;

/* includes */
#include "../Saiph.h"

/* namespace */
using namespace std;

/* keeping track of interesting places */
struct EX_Place {
	int row;
	int col;
	char move;
	int value;
};

/* analyzes the map and finds somewhere to explore */
class ExploreAnalyzer : public Analyzer {
	public:
		/* constructors */
		ExploreAnalyzer(Saiph *saiph);

		/* methods */
		int analyze(int row, int col, char symbol);
		int finish();
		void command();

	private:
		/* variables */
		Saiph *saiph;
		EX_Place places[EX_MAX_PLACES];
		int place_count;
		int best_place;
};
#endif
