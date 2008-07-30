#ifndef WISH_H
/* defines */
#define WISH_H
/* messages */
#define WISH_MESSAGE "  For what do you wish?  "

/* forward declare */
class Wish;

/* includes */
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* monitors health */
class Wish : public Analyzer {
	public:
		/* constructors */
		Wish(Saiph *saiph);

		/* methods */
		void parseMessages(const string &messages);

	private:
		/* variables */
		Saiph *saiph;
		vector<string> wishes;
};
#endif
