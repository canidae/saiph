#ifndef EVENT_RECEIVED_ITEMS_H
#define EVENT_RECEIVED_ITEMS_H

#include <map>
#include "Event.h"
#include "../Item.h"

namespace event {

	class ReceivedItems : public Event {
	public:
		static const int ID;

		ReceivedItems() : Event("ReceivedItems"), _items() {
		}

		virtual ~ReceivedItems() {
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

		virtual void clear() {
			_items.clear();
		}

		virtual void addItem(unsigned char key, const Item &item) {
			_items[key] = item;
		}

	private:
		std::map<unsigned char, Item> _items;
	};
}
#endif
