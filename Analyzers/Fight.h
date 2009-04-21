#ifndef FIGHT_H
#define FIGHT_H

#include <string>
#include <vector>
#include "Analyzer.h"

/* messages */
#define FIGHT_REALLY_ATTACK "Really attack"
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
		std::vector<std::string> thrown;

		unsigned char gotThrown();
		bool moreDangerousThan(const Monster *a, const Monster *b);
	};
}
#endif
