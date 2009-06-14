#ifndef AMULET_H
#define AMULET_H

#include <set>
#include "Analyzer.h"

/* priorities */
#define PRIORITY_AMULET_LOOT 200
#define PRIORITY_AMULET_WEAR 200

class Item;

namespace analyzer {
	class Amulet : public Analyzer {
	public:
		Amulet();

		void onEvent(event::Event *const event);

	private:
		bool wantItem(const Item &item);
		void wearAmulet(const std::set<unsigned char> &keys);
	};
}
#endif
