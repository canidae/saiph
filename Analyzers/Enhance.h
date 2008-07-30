#ifndef ENHANCE_H
/* defines */
#define ENHANCE_H
/* messages */
#define ENHANCE_MESSAGE_CONFIDENT "  You feel more confident in your "
#define ENHANCE_CURRENT_SKILLS "  Current skills:  "
#define ENHANCE_PICK_A_SKILL "  Pick a skill to advance:  "
/* commands */
#define ENHANCE_ENHANCE "#enhance\n"

/* forward declare */
class Enhance;

/* includes */
#include <string>
#include "../Analyzer.h"
#include "../Globals.h"
#include "../Saiph.h"

/* namespace */
using namespace std;

/* monitors health */
class Enhance : public Analyzer {
	public:
		/* constructors */
		Enhance(Saiph *saiph);

		/* methods */
		void parseMessages(const string &messages);

	private:
		/* variables */
		Saiph *saiph;
		bool do_enhance;
		bool got_enhance_menu;
};
#endif
