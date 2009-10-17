#ifndef ANALYZER_LOOT_H
#define ANALYZER_LOOT_H

#include <set>
#include <string>
#include "Analyzer.h"
#include "../Coordinate.h"
#include "../Events/WantItems.h"

#define PRIORITY_LOOT_VISIT 200

namespace event {
	class Event;
}

namespace analyzer {

	class Loot : public Analyzer {
	public:
		Loot();

		void analyze();
		void parseMessages(const std::string &messages);
		void onEvent(event::Event * const event);

	private:
		bool showing_pickup;
		bool showing_drop;
		event::WantItems wi;
		std::set<Coordinate> visit;
	};
}
#endif
