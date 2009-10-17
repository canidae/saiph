#ifndef ANALYZER_DOOR_H
#define ANALYZER_DOOR_H

#include <string>
#include "Analyzer.h"
#include "../Point.h"

/* priorities */
#define PRIORITY_DOOR_LOOT 200
#define PRIORITY_DOOR_OPEN 100

/* door states */
#define DOOR_LOCKED 1
#define DOOR_SHOP_INVENTORY 2

class Item;

namespace analyzer {

	class Door : public Analyzer {
	public:
		Door();

		void analyze();
		void parseMessages(const std::string &messages);
		void onEvent(event::Event * const event);

	private:
		Point position;
		unsigned char unlock_tool_key;
		bool in_a_pit;

		bool wantItem(const Item &item);
	};
}
#endif
