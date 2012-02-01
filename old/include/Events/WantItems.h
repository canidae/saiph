#ifndef EVENT_WANT_ITEMS_H
#define EVENT_WANT_ITEMS_H

#include <map>
#include "Item.h"
#include "Events/Event.h"

/* problems:
 *  - it's clumsy having a "pickup" event and a "drop" event, analyzers need to handle more events
 *  - with drop event, what if one analyzer want to drop an item another analyzer want?
 * solution:
 * create a single event that just ask analyzers if they want the items.
 * in case of pickup, analyzers just selects the items they want.
 * in case of drop, we first "remove" all items in our inventory and let the analyzer "pick up" what they want.
 * the analyzer set the "want" value how many they want of a given item and we set it to -1 when we drop for beatifying */
namespace event {
	class WantItems : public Event {
	public:
		static const int ID;

		WantItems(bool dropping, bool safe_stash);
		virtual ~WantItems();

		virtual int id();
		virtual std::map<unsigned char, Item>& items();
		virtual std::map<unsigned char, Item>& items(const std::map<unsigned char, Item>& items);
		virtual void clear();
		virtual void addItem(unsigned char key, const Item& item);
		virtual bool dropping();
		virtual bool safeStash();

	private:
		const bool _dropping;
		const bool _safe_stash;
		std::map<unsigned char, Item> _items;
	};
}
#endif
