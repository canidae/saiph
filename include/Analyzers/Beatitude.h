#ifndef ANALYZER_BEATITUDE_H
#define ANALYZER_BEATITUDE_H

#include <set>
#include "Analyzers/Analyzer.h"

class Item;

namespace analyzer {

	class Beatitude : public Analyzer {
	public:
		Beatitude();

		void analyze();
		void onEvent(event::Event * const event);

	private:
		std::set<unsigned char> _beatify;
		int _max_priority;
	};
}
#endif
