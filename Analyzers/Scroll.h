#ifndef ANALYZER_SCROLL_H
#define ANALYZER_SCROLL_H

#include <string>
#include <vector>
#include "Analyzer.h"

#define SCROLL_DEBUG_NAME "Scroll] "
/* messages */
#define SCROLL_CALL_END ":  "
#define SCROLL_CALL_SCROLL "  Call a scroll labeled "

class Saiph;

namespace analyzer {
	class Scroll : public Analyzer {
	public:
		Scroll(Saiph* saiph);

		void parseMessages(const std::string& messages);

	private:
		std::vector<std::string> appearance;
		Saiph* saiph;
	};
}
#endif
