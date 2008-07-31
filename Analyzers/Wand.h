#ifndef WAND_H
/* defines */
#define WAND_H
/* priorities */
#define WAND_PICKUP_PRIORITY 425

/* forward declare */
class Wand;

/* includes */
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* monitors health */
class Wand : public Analyzer {
	public:
		/* constructors */
		Wand(Saiph *saiph);

		/* methods */
		void begin();
		void parseMessages(const string &messages);

	private:
		/* variables */
		Saiph *saiph;
		Request req;
};
#endif
