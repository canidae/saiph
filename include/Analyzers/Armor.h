#ifndef ANALYZER_ARMOR_H
#define ANALYZER_ARMOR_H

#include <set>
#include <string>
#include "Analyzers/Analyzer.h"

#define ARMOR_WEAR_PRIORITY 200
#define ARMOR_UNKNOWN_ENCHANTMENT_BONUS 3
/* various polymorph messages */
#define ARMOR_HAVE_NO_FEET "  You have no feet...  "
#define ARMOR_WONT_FIT_HORN " won't fit over your horn"
#define ARMOR_TOO_MANY_HOOVES "  You have too many hooves to wear "

namespace data {
	class Armor;
}

class Item;

namespace analyzer {

	class Armor : public Analyzer {
	public:
		Armor();

		void analyze();
		void parseMessages(const std::string& messages);
		void onEvent(event::Event * const event);

	private:
		std::set<unsigned char> _put_on;

		bool betterThanCurrent(const Item& item);
		int calculateArmorScore(const Item& item, const data::Armor* armor);
	};
}
#endif
