#ifndef EXPLORE_H
#define EXPLORE_H
/* various */
#define EXPLORE_SEARCH_INTERVAL 16 // how many times we should search a square before moving on
#define EXPLORE_FULLY_SEARCHED 255 // when we won't search a square ever again
#define EXPLORE_LEVELS 128 // max amount of unique levels (this should go)
#define EXPLORE_DEBUG_NAME "Explore] "

#include <map>
#include <string>
#include <vector>
#include "Analyzer.h"
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
		unsigned char search[EXPLORE_LEVELS][MAP_ROW_END + 1][MAP_COL_END + 1];
		bool mines_explored;

		unsigned char calculatePointScore(std::map<Point, int>::iterator w, unsigned int *min_moves, int *best_type);
		unsigned char exploreMines();
};
#endif
