#ifndef FOOD_H
/* defines */
#define FOOD_H
/* priorities */
#define FOOD_PICKUP_PRIORITY 550
#define FOOD_EAT_HUNGRY_PRIORITY 350
#define FOOD_EAT_WEAK_PRIORITY 525
#define FOOD_EAT_FAINTING_PRIORITY 750
#define FOOD_PRAY_FOR_FOOD 950

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
		void complete();
		void finish();
		void parseMessages(const string &messages);

	private:
		/* variables */
		Saiph *saiph;
		string command2;
		list<string> eat_order;
		Request req;
};
#endif
