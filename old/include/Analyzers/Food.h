#ifndef ANALYZER_FOOD_H
#define ANALYZER_FOOD_H

#include <map>
#include <set>
#include <string>
#include "Point.h"
#include "Analyzers/Analyzer.h"
#include "Data/Corpse.h"

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
		void parseMessages(const std::string& messages);
		void onEvent(event::Event* const event);

	private:
		std::map<std::string, int> _eat_priority;
		std::set<unsigned char> _food_items;

		bool safeToEat(const std::map<const std::string, const data::Corpse*>::const_iterator& c);
	};
}
#endif
