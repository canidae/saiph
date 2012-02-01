#ifndef ANALYZER_LOOT_H
#define ANALYZER_LOOT_H

#include <set>
#include <string>
#include "Coordinate.h"
#include "Analyzers/Analyzer.h"

#define PRIORITY_LOOT_VISIT 200
#define PRIORITY_LOOT_DROP 150

namespace event {
	class Event;
}

namespace analyzer {

	class Loot : public Analyzer {
	public:
		Loot();

		void analyze();
		void parseMessages(const std::string& messages);
		void onEvent(event::Event* const event);

	private:
		std::set<Coordinate> _visit;
	};
}
#endif
