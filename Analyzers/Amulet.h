#ifndef AMULET_H
#define AMULET_H

#include <vector>
#include "Analyzer.h"

namespace analyzer {
	class Amulet : public Analyzer {
	public:
		static int id;

		Amulet();

		void onEvent(event::Event *const event);

	private:
		void wearAmulet(const std::vector<unsigned char> &keys);
	};
}
#endif
