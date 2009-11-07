#ifndef ANALYZER_ARMOR_H
#define ANALYZER_ARMOR_H

#include <string>
#include "Analyzer.h"

#define ARMOR_UNKNOWN_ENCHANTMENT_BONUS 3
/* various polymorph messages */
#define ARMOR_HAVE_NO_FEET "  You have no feet...  "
#define ARMOR_WONT_FIT_HORN " won't fit over your horn"
#define ARMOR_TOO_MANY_HOOVES "  You have too many hooves to wear "

namespace analyzer {
	class Armor : public Analyzer {
	public:
		Armor();

		void parseMessages(const std::string& messages);
		void onEvent(event::Event * const event);

	private:
		bool _change_armor;
	};
}
#endif
