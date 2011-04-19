#ifndef ANALYZER_SHOP_H
#define ANALYZER_SHOP_H

#include <string>
#include "Analyzers/Analyzer.h"
#include "Coordinate.h"

#define PRIORITY_SHOP_DROP_DIGGING_TOOL 800

#define SHOP_MESSAGE_LEAVE_TOOL "  \"Will you please leave your "
#define SHOP_MESSAGE_LEAVE_TOOL_ANGRY "  \"Leave the "
#define SHOP_MESSAGE_BIT_IT " bit it, you bought it "
#define SHOP_MESSAGE_USAGE_FEE " Usage fee "
#define SHOP_MESSAGE_SPELLBOOK " no free library, cad "
#define SHOP_MESSAGE_SPELLBOOK2 " You owe "
#define SHOP_MESSAGE_FUEL_TAX " That will cost you "
#define PRIORITY_PAY_FOR_ITEMS 400

namespace action {
	class Action;
}

namespace analyzer {
	class Shop : public Analyzer {
	public:
		Shop();

		void analyze();
		void lastChance(action::Action *const);
		void parseMessages(const std::string&);

		void onEvent(event::Event * const event);

	private:
		bool _shopping;
		Coordinate _tentative_shop_door;

		bool inBlockedDoorway();
		int nearShop(const Coordinate&);
		void dropPicks();
		void setShopping(bool);
	};
}
#endif
