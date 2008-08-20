#ifndef FIGHT_H
/* defines */
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

/* forward declare */
class Fight;

/* includes */
#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Monster.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* analyzer for fighting monsters */
class Fight : public Analyzer {
	public:
		/* constructors */
		Fight(Saiph *saiph);

		/* methods */
		void analyze();
		void parseMessages(const string &messages);
		bool request(const Request &request);

	private:
		/* variables */
		Saiph *saiph;
		string command2;
		string command3;
		vector<string> thrown;
		Request req;

		/* methods */
		unsigned char gotThrown();
};
#endif
