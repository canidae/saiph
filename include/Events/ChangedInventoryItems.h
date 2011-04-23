#ifndef EVENT_CHANGED_INVENTORY_ITEMS_H
#define EVENT_CHANGED_INVENTORY_ITEMS_H

#include <set>
#include "Events/Event.h"

namespace event {
	class ChangedInventoryItems : public Event {
	public:
		static const int ID;

		ChangedInventoryItems();
		virtual ~ChangedInventoryItems();

		virtual int id();
		virtual std::set<unsigned char>& keys();
		virtual std::set<unsigned char>& keys(const std::set<unsigned char>& keys);
		virtual void clear();
		virtual void add(unsigned char key);

	private:
		std::set<unsigned char> _keys;
	};
}
#endif
