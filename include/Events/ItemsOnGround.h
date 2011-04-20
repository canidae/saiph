#ifndef EVENT_ITEMS_ON_GROUND_H
#define EVENT_ITEMS_ON_GROUND_H

#include <list>
#include "Item.h"
#include "Events/Event.h"

namespace event {

	class ItemsOnGround : public Event {
	public:
		static const int ID;

		ItemsOnGround(const std::list<Item> items) : Event("ItemsOnGround"), _items(items) {
		}

		virtual ~ItemsOnGround() {
		}

		virtual int id() {
			return ID;
		}

		virtual const std::list<Item>& items() const {
			return _items;
		}

	private:
		const std::list<Item> _items;
	};
}
#endif
