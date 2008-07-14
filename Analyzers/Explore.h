#ifndef EXPLORE_H
/* defines */
#define EXPLORE_H
/* various */
#define EXPLORE_SEARCH_COUNT 24 // how many times we should search a square
/* priorities */
#define EXPLORE_VISIT_CORRIDOR 40
#define EXPLORE_VISIT_OPEN_DOOR 40
#define EXPLORE_VISIT_UNLIT_AREA 40
#define EXPLORE_SEARCH_CORRIDOR_DEAD_END 40
#define EXPLORE_SEARCH_DOOR_DEAD_END 40
#define EXPLORE_SEARCH_CORRIDOR_CORNER 30
#define EXPLORE_SEARCH_ROOM_CORNER 20
#define EXPLORE_SEARCH_WALL 10

/* forward declare */
class Explore;

/* includes */
#include <list>
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
		void command(string *command);
		int finish();
		void inspect(const Point &point, unsigned char symbol);

	private:
		/* variables */
		Saiph *saiph;
		int search[MAX_BRANCHES][MAX_DUNGEON_DEPTH][MAP_ROW_END + 1][MAP_COL_END + 1];
		bool ep_added[MAX_BRANCHES][MAX_DUNGEON_DEPTH][MAP_ROW_END + 1][MAP_COL_END + 1];
		bool visited[MAX_BRANCHES][MAX_DUNGEON_DEPTH][MAP_ROW_END + 1][MAP_COL_END + 1];
		list<Point> explore;
		unsigned char move;
};
#endif
