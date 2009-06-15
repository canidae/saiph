#ifndef ANALYZER_SHOP_H
#define ANALYZER_SHOP_H

#include <string>
#include "Analyzer.h"
#include "../Point.h"
#include "../Request.h"

#define SHOP_DEBUG_NAME "Shop] "

#define SHOP_MESSAGE_LEAVE_TOOL "  \"Will you please leave your "
#define SHOP_MESSAGE_LEAVE_TOOL_ANGRY "  \"Leave the "

class Saiph;

namespace analyzer {
	class Shop : public Analyzer {
	public:
		Shop(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		bool drop_pick_axe;
		bool look_at_ground;
		Request req;
	};
}
#endif
