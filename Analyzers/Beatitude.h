#ifndef BEATITUDE_H
/* defines */
#define BEATITUDE_H
/* BEATITUDE_DROP_ALTAR_MIN says how many items we need at least before we
 * bother dropping them on an altar.
 * BEATITUDE_DROP_ALTAR_ADD_PER_1000_MOVE says how many additional items per
 * move we need before we bother heading towards an altar. we do this so that
 * she's a bit more reluctant to go a long way back to an altar */
#define BEATITUDE_DROP_ALTAR_MIN 8
#define BEATITUDE_DROP_ALTAR_ADD_PER_1000_MOVE 10
/* priorities */
#define BEATITUDE_DROP_ALTAR_PRIORITY 225
/* messages */
#define BEATITUDE_DROP_WHAT_TYPE "  Drop what type of items?  "

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
