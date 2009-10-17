#ifndef ANALYZER_FOOD_H
#define ANALYZER_FOOD_H

#include <map>
#include <set>
#include <string>
#include "Analyzer.h"
#include "../Point.h"
#include "../Data/Corpse.h"

/* how old corpses may be */
#define FOOD_CORPSE_EAT_TIME 30

/* priorities, should be cleaned up */
#define PRIORITY_FOOD_PRAY_FOR_FOOD 950
#define PRIORITY_FOOD_EAT_FAINTING 750
#define PRIORITY_FOOD_EAT_WEAK 725
#define PRIORITY_FOOD_EAT_CORPSE 245
#define PRIORITY_FOOD_EAT_HUNGRY 150
#define PRIORITY_FOOD_LOOT 200

namespace analyzer {

	class Food : public Analyzer {
	public:
		Food();

		void analyze();
		void parseMessages(const std::string &messages);
		void onEvent(event::Event * const event);

	private:
		std::map<Point, unsigned char> prev_monster_loc;
		std::map<Point, int> corpse_loc;
		std::map<std::string, int> eat_priority;
		std::set<unsigned char> food_items;

		bool safeToEat(std::map<std::string, data::Corpse *>::iterator c);
	};
}
#endif
