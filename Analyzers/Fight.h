#ifndef FIGHT_H
#define FIGHT_H

#include <string>
#include <vector>
#include "Analyzer.h"
#include "../Request.h"

#define FIGHT_DEBUG_NAME "Fight] "
/* messages */
#define FIGHT_REALLY_ATTACK "Really attack"
/* used for finding thrown weapons */
#define FIGHT_NOT_CHECKED_THROWN_WEAPONS 0
#define FIGHT_NO_THROWN_WEAPONS 1

class Saiph;
class Monster;

namespace analyzer {
	class Fight : public Analyzer {
	public:
		Fight(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);
		bool request(const Request &request);

	private:
		Saiph *saiph;
		std::string command2;
		std::string command3;
		std::vector<std::string> thrown;
		Request req;

		unsigned char gotThrown();
		bool moreDangerousThan(const Monster *a, const Monster *b);
	};
}
#endif
