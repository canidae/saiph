#ifndef ANALYZER_ENHANCE_H
#define ANALYZER_ENHANCE_H

#include <string>
#include "Analyzers/Analyzer.h"

/* messages */
#define MESSAGE_FEEL_MORE_CONFIDENT "  You feel more confident in your "

namespace analyzer {

	class Enhance : public Analyzer {
	public:
		Enhance();

		void parseMessages(const std::string& messages);
	};
}
#endif
