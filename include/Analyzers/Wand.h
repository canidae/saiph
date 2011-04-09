#ifndef ANALYZER_WAND_H
#define ANALYZER_WAND_H

#include <string>
#include "Analyzers/Analyzer.h"

#define PRIORITY_WAND_ENGRAVE_TEST 200

namespace analyzer {
	class Wand : public Analyzer {
	public:
		Wand();

		void analyze();
		void onEvent(event::Event * const event);
		void actionCompleted(const std::string& messages);

	private:
		unsigned char _engrave_test_wand_key;
	};
}
#endif
