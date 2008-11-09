#include "Enhance.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Enhance::Enhance(Saiph *saiph) : Analyzer("Enhance"), saiph(saiph) {
}

/* methods */
void Enhance::parseMessages(const string &messages) {
	if (messages.find(MESSAGE_FEEL_MORE_CONFIDENT, 0) != string::npos) {
		setCommand(0, PRIORITY_LOOK, ENHANCE, true);
		setCommand(1, PRIORITY_LOOK, CLOSE_PAGE);
		sequence = 0;
	} else if (sequence == 0 && (messages.find(MESSAGE_CURRENT_SKILLS, 0) != string::npos || messages.find(MESSAGE_PICK_A_SKILL, 0) != string::npos)) {
		/* enhance menu just popped up, increase sequence */
		++sequence;
	}
	if (sequence > 0) {
		if (saiph->world->menu) {
			/* showing enhance menu, enhance "a" for now */
			if (messages.find("  a - ", 0) != string::npos)
				setCommand(1, PRIORITY_CONTINUE_ACTION, "a");
			else
				setCommand(1, PRIORITY_CONTINUE_ACTION, CLOSE_PAGE);
		}
	}
}
