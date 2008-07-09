#ifndef EXPLORE_H
/* defines */
#define EXPLORE_H
/* various */
#define EXPLORE_SEARCH_COUNT 24 // how many times we should search a square
#define EXPLORE_PRIORITIES 3 // size of exploration priorities
#define EXPLORE_PRIORITY0 20
#define EXPLORE_PRIORITY1 5
#define EXPLORE_PRIORITY2 5

/* priorities:
 * 1. unlit rooms
 * 2. dead end corridors and doors
 * 3. turning corridor
 * 4. find hidden doors in rooms */

/* forward declare */
class Explore;

/* includes */
#include <vector>
#include "../Globals.h"
#include "../Point.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* analyzes the map and finds somewhere to explore */
class Explore : public Analyzer {
	public:
		/* constructors */
		Explore(Saiph *saiph);

		/* methods */
		void analyze(int row, int col, unsigned char symbol);
		void command(string *command);
		int finish();
		int start();

	private:
		/* variables */
		Saiph *saiph;
		int search[MAX_BRANCHES][MAX_DUNGEON_DEPTH][MAP_ROW_END + 1][MAP_COL_END + 1];
		vector<Point> explore[EXPLORE_PRIORITIES];
		int explore_priority[EXPLORE_PRIORITIES];
		unsigned char move;
};
#endif
