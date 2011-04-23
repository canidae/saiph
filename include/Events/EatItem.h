#ifndef EVENT_EAT_ITEM_H
#define EVENT_EAT_ITEM_H

#include "Events/Event.h"

namespace event {
	class EatItem : public Event {
	public:
		static const int ID;

		EatItem();
		virtual ~EatItem();

		virtual int id();
		virtual unsigned char key();
		virtual int priority();

	private:
		unsigned char _key;
		int _priority;
	};
}
#endif
