#ifndef SCROLL_H
#define SCROLL_H
#define SCROLL_DEBUG_NAME "Scroll] "
/* messages */
#define SCROLL_CALL_END ":  "
#define SCROLL_CALL_SCROLL "  Call a scroll labeled "

#include <string>
#include <vector>
#include "../Analyzer.h"

class Saiph;

class Scroll : public Analyzer {
	public:
		Scroll(Saiph *saiph);

		void parseMessages(const std::string &messages);

	private:
		std::vector<std::string> appearance;
		Saiph *saiph;
};
#endif
