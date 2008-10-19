#ifndef FIGHT_H
#define FIGHT_H
/* messages */
#define FIGHT_REALLY_ATTACK "Really attack"
/* priorities */
#define FIGHT_ATTACK_PRIORITY 450
#define FIGHT_BLUE_E_PRIORITY 30
#define FIGHT_MOVE_PRIORITY 325
#define FIGHT_PICKUP_PRIORITY 425
/* used for finding thrown weapons */
#define FIGHT_NOT_CHECKED_THROWN_WEAPONS 0
#define FIGHT_NO_THROWN_WEAPONS 1

#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Request.h"

class Saiph;

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
};
#endif
