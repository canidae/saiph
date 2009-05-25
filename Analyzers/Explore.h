#ifndef EXPLORE_H
#define EXPLORE_H

#include <string>
#include "Analyzer.h"
#include "../Point.h"

/* search interval */
#define EXPLORE_SEARCH_INTERVAL 16
/* priorities */
#define PRIORITY_EXPLORE_ROGUE 100
#define PRIORITY_EXPLORE_STAIRS_UP 90
#define PRIORITY_EXPLORE_LEVEL 80
#define PRIORITY_EXPLORE_STAIRS_DOWN 70

namespace analyzer {
	class Explore : public Analyzer {
	public:
		Explore();

		void parseMessages(const std::string &messages);
		void analyze();

	private:
		void explorePoint(Point p, unsigned int *min_moves, int *best_type);
	};
}
#endif
