#ifndef FIGHT_H
#define FIGHT_H

#include <string>
#include <vector>
#include "Analyzer.h"

/* used for finding thrown weapons */
#define FIGHT_NOT_CHECKED_THROWN_WEAPONS 0
#define FIGHT_NO_THROWN_WEAPONS 1

namespace event {
	class Event;
}

class Monster;

namespace analyzer {
	class Fight : public Analyzer {
	public:
		Fight();

		void parseMessages(const std::string &messages);
		void analyze();
		void onEvent(event::Event *const event);

	private:
		std::vector<unsigned char> thrown;
	};
}
#endif
