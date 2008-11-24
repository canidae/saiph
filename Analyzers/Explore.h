#ifndef EXPLORE_H
#define EXPLORE_H
/* various */
#define EXPLORE_SEARCH_INTERVAL 16 // how many times we should search a square before moving on
#define EXPLORE_FULLY_SEARCHED 255 // when we won't search a square ever again
/* priorities */
#define EXPLORE_PRIORITY_DESCEND 30
#define EXPLORE_PRIORITY_EXPLORE 50
#define EXPLORE_PRIORITY_SEARCH 20 // priority when stairs not found
#define EXPLORE_PRIORITY_STAIRS_DOWN 40
#define EXPLORE_PRIORITY_STAIRS_UP 60
#define EXPLORE_PRIORITY_ROGUE_STAIRS 70 // priority to find rogue stairs

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
};
#endif
