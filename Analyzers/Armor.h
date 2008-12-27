#ifndef ARMOR_H
#define ARMOR_H

#include <string>
#include <vector>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Request.h"

/* struct for wearing armor */
struct ArmorData {
	int beatitude; // beatitudes we'll accept
	int priority; // wear armor with highest priority, priority += enchantment
	int amount; // how many of this armor we want
	bool keep; // if we should keep this armor even if we find better armor
	std::string name;
};

class Saiph;

class Armor : public Analyzer {
	public:
		Armor(Saiph *saiph);

		void parseMessages(const std::string &messages);
		bool request(const Request &request);

	private:
		Saiph *saiph;
		bool wear_armor;
		std::vector<ArmorData> armor[ARMOR_SLOTS];
		std::string command2;
		Request req;

		bool isCursed(int armor_slot);
		void wearArmor();
};
#endif
