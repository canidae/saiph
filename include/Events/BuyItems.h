#ifndef EVENT_BUY_ITEMS_H
#define EVENT_BUY_ITEMS_H

#include <map>
#include "Item.h"
#include "Events/Event.h"

namespace event {

	class BuyItems : public Event {
	public:
		static const int ID;

		BuyItems(int priority) : Event("BuyItems"), _priority(priority), _items() {
		}

		virtual ~BuyItems() {
		}

		virtual int id() {
			return ID;
		}

		virtual std::map<unsigned char, Item>& items() {
			return _items;
		}

		virtual std::map<unsigned char, Item>& items(const std::map<unsigned char, Item>& items) {
			_items = items;
			return this->items();
		}

		virtual void clear() {
			_items.clear();
		}

		virtual void addItem(unsigned char key, const Item& item) {
			_items[key] = item;
		}

		virtual int priority() {
			return _priority;
		}

	private:
		const bool _priority;
		std::map<unsigned char, Item> _items;
	};
}
#endif
