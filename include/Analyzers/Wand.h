#ifndef ANALYZER_WAND_H
#define ANALYZER_WAND_H

#include <string>
#include "Analyzers/Analyzer.h"

namespace analyzer {
	class Wand : public Analyzer {
	public:
		Wand();

		void parseMessages(const std::string& messages);
		void onEvent(event::Event* const event);
                void actionCompleted(const std::string& messages);

	private:
		unsigned char _engrave_test_wand_key;
	};
}
#endif
