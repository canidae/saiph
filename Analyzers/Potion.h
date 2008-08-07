#ifndef POTION_H
/* defines */
#define POTION_H
/* messages */
#define POTION_CALL_END " potion:  "
#define POTION_CALL_POTION1 "  Call a "
#define POTION_CALL_POTION2 "  Call an "

/* forward declare */
class Potion;

/* includes */
#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* monitors health */
class Potion : public Analyzer {
	public:
		/* constructors */
		Potion(Saiph *saiph);

		/* methods */
		void parseMessages(const string &messages);

	private:
		/* variables */
		vector<string> appearance;
		Saiph *saiph;
};
#endif
