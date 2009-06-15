#ifndef FIGHT_ANALYZER_H
#define FIGHT_ANALYZER_H

#include <set>
#include <string>
#include "Analyzer.h"

#define PRIORITY_FIGHT_MELEE_MAX 300
#define PRIORITY_FIGHT_MELEE_MIN 100
#define PRIORITY_FIGHT_MOVE_MAX 250
#define PRIORITY_FIGHT_MOVE_MIN 50
#define PRIORITY_FIGHT_THROW_MAX 250
#define PRIORITY_FIGHT_THROW_MIN 100

namespace event {
	class Event;
}

class Monster;

namespace analyzer {
	class Fight : public Analyzer {
	public:
		Fight();

		void analyze();
		void onEvent(event::Event *const event);

	private:
		std::set<std::string> projectiles; // weapons we can throw
		std::set<unsigned char> projectile_slots; // slots where we got weapons we should throw
	};
}
#endif
