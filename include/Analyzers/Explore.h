#ifndef ANALYZER_EXPLORE_H
#define ANALYZER_EXPLORE_H

#include <map>
#include <string>
#include "Coordinate.h"
#include "Point.h"
#include "Analyzers/Analyzer.h"

/* search interval */
#define EXPLORE_SEARCH_INTERVAL 16
/* base priority */
#define PRIORITY_EXPLORE 70


// find the endpoint of an unknown stair
#define RRANK_STAIR (-1)
// dark rooms, unexplored area, dead end searching
#define RRANK_EXPLORE 0
// searching walls (or floor, if questish)
// higher values occur if they are already highly searched
#define RRANK_SEARCH_MIN 1


// Explore's general principle is to assign a rank to tiles, where low ranks are basic exploration and higher represent increasingly desparate searching.  Explore will exhaust tiles available at one rank and any distance before moving on to the next rank.  Order of level exploration is controlled by level-wide modifications of rank, for instance to reward exploring levels 2-4 when the Mines are not yet found.
// Ranks are either cooked, which depend on depth and level priority factors, or raw, determined (almost) only by level topology.
// Because the relative distances of tiles on other levels doesn't change in normal movement, we can do a certain amount of caching: only the current level needs to be scanned every turn.
// This may break once saiph starts finding the Orb of Weight.

class Tile;

namespace analyzer {
	struct ExploreFocus {
		Point where;
		int rank;
		std::string purpose;
		unsigned char direction;

		std::string describe() const;
	};

	class Explore : public Analyzer {
	public:
		Explore();

		void analyze();

	private:
		std::map<int, ExploreFocus> _explore_levels;

		static ExploreFocus analyzeLevel();
		static void considerPoint(ExploreFocus& best, Point p);
		static void addOption(ExploreFocus& best, int rank, const Point& p, unsigned char direction, const std::string& purpose);
	};
}
#endif
