#ifndef ANALYZER_FOOD_H
#define ANALYZER_FOOD_H

/* some text */
#define FOOD_EAT_IT_1 "  There is "
#define FOOD_EAT_IT_2 " here; eat it? "
#define FOOD_EAT_ONE_1 "  There are "
#define FOOD_EAT_ONE_2 " here; eat one? "
#define FOOD_IS_DESTROYED " is destroyed!  "
#define FOOD_IS_KILLED " is killed!  "
#define FOOD_YOU_DESTROY "  You destroy the "
#define FOOD_YOU_KILL "  You kill the "
/* how old corpses may be */
#define FOOD_CORPSE_EAT_TIME 30

#include <map>
#include <string>
#include "Analyzer.h"

namespace analyzer {
	class Food : public Analyzer {
	public:
		Food();

		void analyze();
		void parseMessages(const std::string &messages);
		void onEvent(event::Event *const event);

	private:
		std::map<Point, unsigned char> prev_monster_loc;
		std::map<Point, int> corpse_loc;

		bool safeToEat(const std::string &corpse);
	};
}
#endif
