#ifndef FOOD_H
/* defines */
#define FOOD_H
/* priorities */
#define FOOD_LOOT_PRIORITY 60
#define FOOD_EAT_PRIORITY 60

/* forward declare */
class Food;

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
class Food : public Analyzer {
	public:
		/* constructors */
		Food(Saiph *saiph);

		/* methods */
		void command(string *command);
		void finish();
		void parseMessages(const string &messages);
		bool request(const Request &request);

	private:
		/* variables */
		Saiph *saiph;
		string action;
		unsigned char eat_key;
		list<string> eat_order;
		Request req;
};
#endif
