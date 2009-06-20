#ifndef EVENT_WANT_ITEMS_H
#define EVENT_WANT_ITEMS_H

#include <map>
#include "Event.h"
#include "../Item.h"

/* problems:
 *  - it's clumsy having a "pickup" event and a "drop" event, analyzers need to handle more events
 *  - with drop event, what if one analyzer want to drop an item another analyzer want?
 * solution:
 * create a single event that just ask analyzers if they want the items.
 * in case of pickup, analyzers just selects the items they want.
 * in case of drop, we first select every item to be dropped, then analyzers select what _not_ to be dropped */
namespace event {
	class WantItems : public Event {
	public:
		static int id;
		std::map<unsigned char, Item> items;

		WantItems() : items() {}
		virtual ~WantItems() {}

		virtual int getID() {return id;}
		virtual std::string getName() {return "WantItems";}
	};
}
#endif
