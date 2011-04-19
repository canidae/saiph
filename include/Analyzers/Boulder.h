#ifndef ANALYZER_BOULDER_H
#define ANALYZER_BOULDER_H

#include <string>
#include "Point.h"
#include "Analyzers/Analyzer.h"

/* priorities */
#define PRIORITY_BOULDER_DESTROY 100

class Item;

namespace analyzer {

	class Boulder : public Analyzer {
	public:
		Boulder();

		void analyze();
		void onEvent(event::Event* const event);

	private:
		unsigned char _pick_key;
	};
}
#endif
