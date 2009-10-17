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
 * in case of drop, we first "remove" all items in our inventory and let the analyzer "pick up" what they want.
 * the analyzer set the "want" value how many they want of a given item and we set it to -1 when we drop for beatifying */
namespace event {

	class WantItems : public Event {
	public:
		static const int ID;

		WantItems() : Event("WantItems"), _items(), _want() {
		}

		virtual ~WantItems() {
		}

		virtual int id() {
			return ID;
		}

		virtual std::map<unsigned char, Item> &items() {
			return _items;
		}

		virtual std::map<unsigned char, Item> &items(const std::map<unsigned char, Item> &items) {
			_items = items;
			return this->items();
		}

		virtual std::map<unsigned char, int> &want() {
			return _want;
		}

		virtual std::map<unsigned char, int> &want(const std::map<unsigned char, int> &want) {
			_want = want;
			return this->want();
		}

		virtual void clear() {
			_items.clear();
			_want.clear();
		}

		virtual void addItem(unsigned char key, const Item &item, int want) {
			_items[key] = item;
			_want[key] = want;
		}

	private:
		std::map<unsigned char, Item> _items;
		std::map<unsigned char, int> _want;
	};
}
#endif
