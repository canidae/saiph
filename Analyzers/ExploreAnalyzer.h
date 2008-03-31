#ifndef EXPLOREANALYZER_H
/* defines */
#define EXPLOREANALYZER_H
/* various */
#define EX_MAX_PLACES 128

#define EX_DEAD_END 20 // corridor that just ends
#define EX_UNLIT_ROOM 20 // room where we haven't seen the floor
#define EX_TURN 15 // corridor that turns
#define EX_ROOM_EXIT 10 // path leading out of room
#define EX_EXTRA_SEARCH 5 // frantic searching for door

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
		virtual void analyze(int row, int col, char symbol);
		virtual void finish();

	private:
		/* variables */
		EX_Place places[EX_MAX_PLACES];
		int place_count;
		Saiph *saiph;
};
#endif
