#ifndef AMULET_H
#define AMULET_H

#include <set>
#include "Analyzer.h"

/* priorities */
#define PRIORITY_AMULET_WEAR 200

namespace analyzer {
	class Amulet : public Analyzer {
	public:
		Amulet();

		void onEvent(event::Event *const event);

	private:
		void wearAmulet(const std::set<unsigned char> &keys);
	};
}
#endif
