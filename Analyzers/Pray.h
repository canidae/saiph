#ifndef PRAY_H
/* defines */
#define PRAY_H
/* turns between safe to pray */
#define PRAY_PRAYER_TIMEOUT 1000

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
		void command(string *command);
		void parseMessages(const string &messages);
		bool request(const Request &request);

	private:
		/* variables */
		Saiph *saiph;
		string action;
		int last_pray_turn;
};
#endif
