#ifndef ANALYZER_PRAY_H
#define ANALYZER_PRAY_H

#include <string>
#include "Analyzer.h"

/* turns between safe to pray */
#define PRAY_PRAYER_TIMEOUT 1000

namespace event {
	class Event;
}

namespace analyzer {
	class Pray : public Analyzer {
	public:
		Pray();

		void parseMessages(const std::string &messages);
		void onEvent(event::Event *const event);

	private:
		int last_pray_turn;
	};
}
#endif
