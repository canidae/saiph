#ifndef SHOP_H
#define SHOP_H

#define SHOP_MESSAGE_LEAVE_TOOL "  \"Will you please leave your "
#define SHOP_MESSAGE_LEAVE_TOOL_ANGRY "  \"Leave the "

#include <string>
#include "../Analyzer.h"
#include "../Point.h"

class Saiph;

class Shop : public Analyzer {
	public:
		Shop(Saiph *saiph);

		void analyze();
		void parseMessages(const std::string &messages);

	private:
		Saiph *saiph;
		bool drop_pick_axe;
		bool look_at_ground;
};
#endif
