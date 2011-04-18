#ifndef ANALYZER_SHOP_H
#define ANALYZER_SHOP_H

#include <string>
#include "Analyzers/Analyzer.h"

#define SHOP_MESSAGE_LEAVE_TOOL "  \"Will you please leave your "
#define SHOP_MESSAGE_LEAVE_TOOL_ANGRY "  \"Leave the "
#define SHOP_MESSAGE_BIT_IT " bit it, you bought it "
#define SHOP_MESSAGE_USAGE_FEE " Usage fee "
#define SHOP_MESSAGE_SPELLBOOK " no free library, cad "
#define SHOP_MESSAGE_SPELLBOOK2 " You owe "
#define SHOP_MESSAGE_FUEL_TAX " That will cost you "
#define PRIORITY_PAY_FOR_ITEMS 400

namespace analyzer {
	class Shop : public Analyzer {
	public:
		Shop();

		void analyze();
		void parseMessages(const std::string&);
	private:
		int payed;
	};
}
#endif
