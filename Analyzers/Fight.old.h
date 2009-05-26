#ifndef FIGHT_H
#define FIGHT_H

#include <set>
#include <string>
#include "Analyzer.h"

#define PRIORITY_FIGHT_ENGULFED 200

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
		std::set<unsigned char> thrown;
	};
}
#endif
