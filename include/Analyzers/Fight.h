#ifndef ANALYZER_FIGHT_H
#define ANALYZER_FIGHT_H

#include <set>
#include <map>
#include <string>
#include "Analyzers/Analyzer.h"
#include "Point.h"

#define PRIORITY_FIGHT_MELEE_MAX 400
#define PRIORITY_FIGHT_MELEE_MIN 250
#define PRIORITY_FIGHT_MOVE_MAX 250
#define PRIORITY_FIGHT_MOVE_MIN 150
#define PRIORITY_FIGHT_THROW_MAX 300
#define PRIORITY_FIGHT_THROW_MIN 200
#define PRIORITY_FIGHT_POSITION_BOSS 250
#define PRIORITY_FIGHT_POSITION_BOSS_LURE 450

#define BOSS_PATIENCE 50

namespace event {
	class Event;
}

class Item;

namespace analyzer {

	class Fight : public Analyzer {
	public:
		Fight();

		void analyze();
		void parseMessages(const std::string&);
		void onEvent(event::Event* const event);

	private:
		std::set<unsigned char> _projectile_slots; // slots where we got weapons we should throw
		std::map<std::string, int> _boss_last_seen;
		int _boss_waiting_since;
		int _boss_waiting_level;

		int pointVulnerability(const Point& pt, int efactor, bool crowd, bool stalk, bool fast);
		void checkPanic();
	};
}
#endif
