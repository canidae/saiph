#ifndef EXPLORE_H
#define EXPLORE_H
/* various */
#define EXPLORE_SEARCH_INTERVAL 16 // how many times we should search a square before moving on
#define EXPLORE_FULLY_SEARCHED 255 // when we won't search a square ever again
/* priorities */
#define EXPLORE_PRIORITY_EXPLORE 60 // priority when exploring a level
#define EXPLORE_PRIORITY_ROGUE_STAIRS 70 // priority to find rogue stairs
#define EXPLORE_PRIORITY_SEARCH 20 // priority when searching desperately
#define EXPLORE_PRIORITY_STAIRS_DOWN 40 // priority for exploring unknown stairs down
#define EXPLORE_PRIORITY_STAIRS_UP 50 // priority for exploring unknown stairs up
#define EXPLORE_PRIORITY_TRAVEL 30 // priority to travel somewhere

#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Point.h"

class Saiph;

class Explore : public Analyzer {
	public:
		Explore(Saiph *saiph);

		void analyze();
		void complete();
		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		std::vector<int> explore_priorities;
		unsigned char search[LEVELS][MAP_ROW_END + 1][MAP_COL_END + 1];
		bool mines_explored;

		unsigned char exploreMines();
};
#endif
