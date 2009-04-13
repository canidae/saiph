#ifndef _RECEIVED_ITEMS_H
#define	_RECEIVED_ITEMS_H

#include <vector>
#include "Event.h"
#include "../Item.h"

namespace event {
	class ReceivedItems {
	public:
		static const int id;
		std::vector<Item> items;

		ReceivedItems();
		virtual ~ReceivedItems();

		virtual int getID();
	};
}
#endif
