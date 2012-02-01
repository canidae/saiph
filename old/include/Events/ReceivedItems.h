#ifndef EVENT_RECEIVED_ITEMS_H
#define EVENT_RECEIVED_ITEMS_H

#include <map>
#include "Item.h"
#include "Events/Event.h"

namespace event {
	class ReceivedItems : public Event {
	public:
		static const int ID;

		ReceivedItems();
		virtual ~ReceivedItems();

		virtual int id();
		virtual std::map<unsigned char, Item>& items();
		virtual std::map<unsigned char, Item>& items(const std::map<unsigned char, Item>& items);
		virtual void clear();
		virtual void addItem(unsigned char key, const Item& item);

	private:
		std::map<unsigned char, Item> _items;
	};
}
#endif
