#ifndef _RECEIVED_ITEMS_H
#define _RECEIVED_ITEMS_H

#include <vector>
#include "Event.h"

namespace event {
	class ReceivedItems {
	public:
		static const int id;
		std::vector<unsigned char> item_keys;

		ReceivedItems(const std::vector<unsigned char> &item_keys) : item_keys(item_keys) {}
		virtual ~ReceivedItems() {}

		virtual int getID() {return id;}
		virtual std::string getName() {return "ReceivedItems";}
	};
}
#endif
