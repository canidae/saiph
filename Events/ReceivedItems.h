#ifndef _RECEIVED_ITEMS_H
#define	_RECEIVED_ITEMS_H

#include "Event.h"

namespace event {
	class ReceivedItems {
	public:
		static const int id;

		ReceivedItems();
		virtual ~ReceivedItems();

		virtual int getID();
	};
}
#endif
