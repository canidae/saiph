#ifndef EVENT_ITEMS_ON_GROUND_H
#define EVENT_ITEMS_ON_GROUND_H

#include <list>
#include "Event.h"
#include "../Item.h"

namespace event {

	class ItemsOnGround : public Event {
	public:
		static const int ID;

		ItemsOnGround() : Event("ItemsOnGround"), _items() {
		}

		virtual ~ItemsOnGround() {
		}

		virtual int id() {
			return ID;
		}

		virtual std::list<Item> &items() {
			return _items;
		}

		virtual std::list<Item> &items(const std::list<Item> &items) {
			_items = items;
			return this->items();
		}

	private:
		std::list<Item> _items;
	};
}
#endif
