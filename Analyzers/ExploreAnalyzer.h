#ifndef EXPLOREANALYZER_H
/* defines */
#define EXPLOREANALYZER_H
/* various */
#define EX_MAX_PLACES 128
#define EX_DEAD_END_CORRIDOR 20 // think of a corridor that ends
#define EX_DEAD_END_ROOM 5 // think of a path in a maze that ends
#define EX_TURN_CORRIDOR 15 // think of a turn in a corridor
#define EX_TURN_ROOM 10 // think of a corner in a room
#define EX_STRAIGHT_CORRIDOR 5 // think of a corridor going straight ahead
#define EX_STRAIGHT_ROOM 5 // think of straight path in a maze
#define EX_TCROSS_CORRIDOR 5 // think of 3 corridors joining
#define EX_TCROSS_ROOM 10 // think of a straight wall

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
