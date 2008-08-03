#ifndef PRAY_H
/* defines */
#define PRAY_H
/* turns between safe to pray */
#define PRAY_PRAYER_TIMEOUT 1000
/* messages */
#define PRAY_FINISHED_PRAYING "  You finish your prayer.  "

/* forward declare */
class Pray;

/* includes */
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* monitors health */
class Pray : public Analyzer {
	public:
		/* constructors */
		Pray(Saiph *saiph);

		/* methods */
		void parseMessages(const string &messages);
		bool request(const Request &request);

	private:
		/* variables */
		Saiph *saiph;
		int last_pray_turn;
};
#endif
