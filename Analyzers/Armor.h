#ifndef ARMOR_H
/* defines */
#define ARMOR_H
/* priorities */
#define ARMOR_PICKUP_PRIORITY 425
#define ARMOR_WEAR_PRIORITY 225
/* messages */
#define ARMOR_WHAT_TO_WEAR "  What do you want to wear? "

/* forward declare */
class Armor;

/* includes */
#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Request.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* struct for wearing armor */
struct WearArmor {
	int beatitude;
	string name;
};

/* analyzer for wearing armor */
class Armor : public Analyzer {
	public:
		/* constructors */
		Armor(Saiph *saiph);

		/* methods */
		void parseMessages(const string &messages);
		bool request(const Request &request);

	private:
		/* variables */
		Saiph *saiph;
		vector<WearArmor> armor[ARMOR_SLOTS];
		string command2;
		Request req;

		/* methods */
		void wearArmor();
};
#endif
