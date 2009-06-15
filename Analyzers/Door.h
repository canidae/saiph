#ifndef DOOR_H
#define DOOR_H

#include <string>
#include "Analyzer.h"
#include "../Point.h"

/* priorities */
#define PRIORITY_DOOR_LOOT 200
#define PRIORITY_DOOR_OPEN 100

/* messages */
#define DOOR_DOOR_LOCKED "  This door is locked.  "
#define DOOR_BREAK_SHOP_DOOR "  \"Cad!  You did 400 zorkmids worth of damage!\"  Pay?  [yn] (n)  "
#define DOOR_CLOSED_FOR_INVENTORY "\"Closed for inventory.\""
#define DOOR_SUCCEED_UNLOCKING "  You succeed in unlocking the door.  "

/* door states */
#define DOOR_LOCKED 1
#define DOOR_SHOP_INVENTORY 2

class Item;
class Saiph;

namespace analyzer {
	class Door : public Analyzer {
	public:
		Door();

		void analyze();
		void parseMessages(const std::string &messages);
		void onEvent(event::Event *const event);

	private:
		Point position;
		unsigned char unlock_tool_key;
		bool in_a_pit;

	bool wantItem(const Item &item);
	};
}
#endif
