#ifndef ZORKMID_H
/* defines */
#define ZORKMID_H
/* priorities */
#define ZORKMID_LOOT_PRIORITY 275
/* gold */
#define ZORKMID_GOLD_PIECE "gold piece"


/* forward declare */
class Zorkmid;

/* includes */
#include <list>
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* monitors health */
class Zorkmid : public Analyzer {
	public:
		/* constructors */
		Zorkmid(Saiph *saiph);

		/* methods */
		void begin();
		void complete();
		void parseMessages(const string &messages);

	private:
		/* variables */
		Saiph *saiph;
		Request req;
};
#endif
