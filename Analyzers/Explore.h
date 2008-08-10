#ifndef EXPLORE_H
/* defines */
#define EXPLORE_H
/* various */
#define EXPLORE_SEARCH_COUNT 24 // how many times we should search a square
/* priorities */
#define EXPLORE_VISIT_CORRIDOR 60
#define EXPLORE_VISIT_OPEN_DOOR 60
#define EXPLORE_VISIT_UNLIT_AREA 60
#define EXPLORE_VISIT_UNKNOWN_TILE 25
#define EXPLORE_SEARCH_CORRIDOR_DEAD_END 60
#define EXPLORE_SEARCH_DOOR_DEAD_END 60
#define EXPLORE_SEARCH_ROOM_CORNER 25
#define EXPLORE_SEARCH_CORRIDOR_CORNER 10
#define EXPLORE_SEARCH_WALL 10
#define EXPLORE_UNKNOWN_STAIRS 35
/* this should go */
#define MAX_DUNGEON_DEPTH 64

/* forward declare */
class Explore;

/* includes */
#include <list>
#include <string>
#include "../Analyzer.h"
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
		void analyze();
		void complete();
		void inspect(const Point &point);

	private:
		/* variables */
		Saiph *saiph;
		int search[MAX_DUNGEON_DEPTH][MAP_ROW_END + 1][MAP_COL_END + 1];
		bool visited[MAX_DUNGEON_DEPTH][MAP_ROW_END + 1][MAP_COL_END + 1];
		list<Point> explore;
};
#endif
