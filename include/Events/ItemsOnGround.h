#ifndef EVENT_ITEMS_ON_GROUND_H
#define EVENT_ITEMS_ON_GROUND_H

#include <list>
#include "Item.h"
#include "Events/Event.h"

namespace event {
	class ItemsOnGround : public Event {
	public:
		static const int ID;

		ItemsOnGround(const std::list<Item>& items);
		virtual ~ItemsOnGround();

		virtual int id();
		virtual const std::list<Item>& items() const;

	private:
		const std::list<Item> _items;
	};
}
#endif
