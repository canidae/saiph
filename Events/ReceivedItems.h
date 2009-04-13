#ifndef _RECEIVED_ITEMS_H
#define	_RECEIVED_ITEMS_H

#include "Event.h"

namespace event {
	class ReceivedItems {
	public:
		ReceivedItems();
		virtual ~ReceivedItems();

		virtual int getID();

	private:
		static const int id;
	};
}
#endif
