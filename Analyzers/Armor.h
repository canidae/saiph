#ifndef ARMOR_H
#define ARMOR_H
/* priorities */
#define ARMOR_PICKUP_PRIORITY 425
#define ARMOR_WEAR_PRIORITY 225
/* messages */
#define ARMOR_WHAT_TO_WEAR "  What do you want to wear? "
#define ARMOR_WHAT_TO_TAKE_OFF "  What do you want to take off? "
#define ARMOR_YOU_WERE_WEARING "  You were wearing "

#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Request.h"

/* struct for wearing armor */
struct WearArmor {
	int beatitude;
	std::string name;
};

class Saiph;

class Armor : public Analyzer {
	public:
		Armor(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);
		bool request(const Request &request);

	private:
		Saiph *saiph;
		bool wear_more;
		std::vector<WearArmor> armor[ARMOR_SLOTS];
		std::string command2;
		Request req;

		void wearArmor();
};
#endif
