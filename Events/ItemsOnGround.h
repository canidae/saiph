#ifndef EVENT_ITEMS_ON_GROUND_H
#define EVENT_ITEMS_ON_GROUND_H

#include <list>
#include "Event.h"
#include "../Item.h"

namespace event {
	class ItemsOnGround : public Event {
	public:
		static int id;
		std::list<Item> items;

		ItemsOnGround() : items() {}
		virtual ~ItemsOnGround() {}

		virtual int getID() {return id;}
		virtual std::string getName() {return "ItemsOnGround";}
	};
}
#endif
