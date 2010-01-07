#ifndef ANALYZER_FIGHT_H
#define ANALYZER_FIGHT_H

#include <set>
#include <string>
#include "Analyzers/Analyzer.h"

#define PRIORITY_FIGHT_MELEE_MAX 400
#define PRIORITY_FIGHT_MELEE_MIN 250
#define PRIORITY_FIGHT_MOVE_MAX 250
#define PRIORITY_FIGHT_MOVE_MIN 150
#define PRIORITY_FIGHT_THROW_MAX 300
#define PRIORITY_FIGHT_THROW_MIN 200

namespace event {
	class Event;
}

class Item;

namespace analyzer {

	class Fight : public Analyzer {
	public:
		Fight();

		void analyze();
		void onEvent(event::Event * const event);

	private:
		std::set<unsigned char> _projectile_slots; // slots where we got weapons we should throw
	};
}
#endif
