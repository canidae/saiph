#ifndef ANALYZER_ENHANCE_H
#define ANALYZER_ENHANCE_H

#include <string>
#include "Analyzer.h"

/* messages */
#define ENHANCE_MESSAGE_CONFIDENT "  You feel more confident in your "
#define ENHANCE_CURRENT_SKILLS "  Current skills:  "
#define ENHANCE_PICK_A_SKILL "  Pick a skill to advance:  "
/* commands */
#define ENHANCE_ENHANCE "#enhance\n"

class Saiph;

namespace analyzer {
	class Enhance : public Analyzer {
	public:
		Enhance(Saiph* saiph);

		void parseMessages(const std::string& messages);

	private:
		Saiph* saiph;
		bool do_enhance;
		bool got_enhance_menu;
	};
}
#endif
