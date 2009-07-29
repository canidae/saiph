#ifndef ANALYZER_FOOD_H
#define ANALYZER_FOOD_H

/* how old corpses may be */
#define FOOD_CORPSE_EAT_TIME 30

#include <map>
#include <set>
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
		std::map<std::string, int> eat_priority;
		std::set<unsigned char> food_items;

		bool safeToEat(std::map<std::string, data::Corpse *>::iterator c);
	};
}
#endif
