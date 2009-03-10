#ifndef ARMOR_H
#define ARMOR_H

#define ARMOR_UNKNOWN_ENCHANTMENT_BONUS 3
/* various polymorph messages */
#define ARMOR_HAVE_NO_FEET "  You have no feet...  "
#define ARMOR_WONT_FIT_HORN " won't fit over your horn"
#define ARMOR_TOO_MANY_HOOVES "  You have too many hooves to wear "

#include <string>
#include <vector>
#include <deque>
#include <utility>
#include "../Item.h"
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Request.h"
#include "../Data/ArmorData.h"

/* struct for wearing armor */
//struct ArmorData {
//	int beatitude; // beatitudes we'll accept
//	int priority; // wear armor with highest priority, priority += enchantment
//	bool keep; // if we should keep this armor even if we find better armor
//	std::string name;
//};

class Saiph;

class Armor : public Analyzer {
	public:
		Armor(Saiph *saiph);

		void parseMessages(const std::string &messages);
		bool request(const Request &request);

	private:
		Saiph *saiph;
		bool wear_armor;
		int carry_amount[ARMOR_SLOTS];
		bool can_wear[ARMOR_SLOTS];
		std::vector<ArmorData> armor[ARMOR_SLOTS];
		std::string command2;
		Request req;
		int last_armor_type;
		bool last_polymorphed;

		Item * wearingInSlot(int armor_slot);
		bool isCursed(int armor_slot);
		int rank(const std::vector<Item> &armor);
		void wearArmor();
		void makeArmorCombinations(const std::vector<Item> choices[], bool slotCursed[], int curSlot, std::vector<Item> &curVec, std::vector< std::vector<Item> > &listOfCombos);
		void resetCanWear();
};
#endif
