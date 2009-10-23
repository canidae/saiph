#ifndef EVENT_CHANGED_INVENTORY_ITEMS_H
#define EVENT_CHANGED_INVENTORY_ITEMS_H

#include <set>
#include "Event.h"

namespace event {

	class ChangedInventoryItems : public Event {
	public:
		static const int ID;

		ChangedInventoryItems() : Event("ChangedInventoryItems"), _keys() {
		}

		virtual ~ChangedInventoryItems() {
		}

		virtual const int& id() {
			return ID;
		}

		virtual std::set<unsigned char>& keys() {
			return _keys;
		}

		virtual std::set<unsigned char>& keys(const std::set<unsigned char>& keys) {
			_keys = keys;
			return this->keys();
		}

		virtual void clear() {
			_keys.clear();
		}

		virtual void add(const unsigned char& key) {
			_keys.insert(key);
		}

	private:
		std::set<unsigned char> _keys;
	};
}
#endif
