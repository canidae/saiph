#ifndef _RECEIVED_ITEMS_H
#define _RECEIVED_ITEMS_H

#include <map>
#include "Event.h"
#include "../Item.h"

namespace event {
	class ReceivedItems : public Event {
	public:
		static int id;
		std::map<unsigned char, Item> items;

		ReceivedItems() : items() {}
		virtual ~ReceivedItems() {}

		virtual int getID() {return id;}
		virtual std::string getName() {return "ReceivedItems";}
	};
}
#endif
