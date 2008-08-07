#ifndef SCROLL_H
/* defines */
#define SCROLL_H
/* messages */
#define SCROLL_CALL_END ":  "
#define SCROLL_CALL_SCROLL "  Call a scroll labeled "

/* forward declare */
class Scroll;

/* includes */
#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* monitors health */
class Scroll : public Analyzer {
	public:
		/* constructors */
		Scroll(Saiph *saiph);

		/* methods */
		void parseMessages(const string &messages);

	private:
		/* variables */
		vector<string> appearance;
		Saiph *saiph;
};
#endif
