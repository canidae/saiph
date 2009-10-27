#ifndef ANALYZER_SHOP_H
#define ANALYZER_SHOP_H

#include <string>
#include "Analyzer.h"

#define SHOP_MESSAGE_LEAVE_TOOL "  \"Will you please leave your "
#define SHOP_MESSAGE_LEAVE_TOOL_ANGRY "  \"Leave the "

namespace analyzer {

	class Shop : public Analyzer {
	public:
		Shop();

		void analyze();
		void parseMessages(const std::string& messages);

	private:
		bool drop_pick_axe;
		bool look_at_ground;
	};
}
#endif
