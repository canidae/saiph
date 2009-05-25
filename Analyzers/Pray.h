#ifndef PRAY_H
#define PRAY_H

#include <string>
#include "Analyzer.h"

/* turns between safe to pray */
#define PRAY_PRAYER_TIMEOUT 1000
/* messages */
#define PRAY_FINISHED_PRAYING "  You finish your prayer.  "

class Request;
class Saiph;

namespace analyzer {
	class Pray : public Analyzer {
	public:
		Pray(Saiph *saiph);

		void parseMessages(const std::string &messages);
		bool request(const Request &request);

	private:
		Saiph *saiph;
		int last_pray_turn;
	};
}
#endif
