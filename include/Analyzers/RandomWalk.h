#ifndef ANALYZER_RANDOM_WALK_H
#define ANALYZER_RANDOM_WALK_H

#include "Analyzers/Analyzer.h"

/* priorities */
#define PRIORITY_RANDOM_WALK 1

class Item;

namespace analyzer {
	class RandomWalk : public Analyzer {
	public:
		RandomWalk();

		void analyze();
	};
}
#endif
