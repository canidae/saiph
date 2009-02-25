#ifndef ENHANCE_H
#define ENHANCE_H
/* messages */
#define ENHANCE_MESSAGE_CONFIDENT "  You feel more confident in your "
#define ENHANCE_CURRENT_SKILLS "  Current skills:  "
#define ENHANCE_PICK_A_SKILL "  Pick a skill to advance:  "
/* commands */
#define ENHANCE_ENHANCE "#enhance\n"

#include <string>
#include "../Analyzer.h"

class Saiph;

class Enhance : public Analyzer {
	public:
		Enhance(Saiph *saiph);

		void analyze(const std::string &messages);

	private:
		Saiph *saiph;
		bool do_enhance;
		bool got_enhance_menu;
};
#endif
