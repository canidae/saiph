#ifndef ANALYZER_DONATE_H
#define ANALYZER_DONATE_H

#include "Coordinate.h"
#include "Analyzers/Analyzer.h"

#define DONATE_PRIORITY 150

namespace analyzer {

	class Donate : public Analyzer {
	public:
		Donate();

		void analyze();
		void onEvent(event::Event * const event);

	private:
		Coordinate _priest_loc;
	};
}
#endif
