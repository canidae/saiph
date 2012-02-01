#ifndef ANALYZER_DOOR_H
#define ANALYZER_DOOR_H

#include <string>
#include "Point.h"
#include "Analyzers/Analyzer.h"

/* priorities */
#define PRIORITY_DOOR_OPEN 100

/* door states, not mutually exclusive */
#define DOOR_IS_LOCKED 1
#define DOOR_IS_SHOP 2

class Item;

namespace analyzer {

	class Door : public Analyzer {
	public:
		Door();

		void analyze();
		void parseMessages(const std::string& messages);
		void onEvent(event::Event* const event);

	private:
		Point _position;
		unsigned char _unlock_tool_key;

		bool wantItem(const Item& item);
		int getDoorFlags(const Point& pt);
	};
}
#endif
