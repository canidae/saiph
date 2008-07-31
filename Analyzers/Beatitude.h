#ifndef BEATITUDE_H
/* defines */
#define BEATITUDE_H
/* least amount of items with unknown beatitude before we head to altar */
#define BEATITUDE_DROP_ALTAR_MIN 3
/* priorities */
#define BEATITUDE_DROP_ALTAR_PRIORITY 225
/* messages */
#define BEATITUDE_DROP_WHAT_TYPE "  Drop what type of items?  "
#define BEATITUDE_DROP_WHICH_ITEMS "  What would you like to drop?  "

/* forward declare */
class Beatitude;

/* includes */
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* monitors health */
class Beatitude : public Analyzer {
	public:
		/* constructors */
		Beatitude(Saiph *saiph);

		/* methods */
		void begin();
		void parseMessages(const string &messages);

	private:
		/* variables */
		Saiph *saiph;
		bool check_beatitude;
		bool got_drop_menu;
};
#endif
