#include "Wish.h"
#include "../Saiph.h"
#include "../Globals.h"

using namespace std;

/* constructors/destructor */
Wish::Wish(Saiph *saiph) : Analyzer("Wish"), saiph(saiph) {
	wishes.push_back("blessed greased fixed +3 gray dragon scale mail");
}

/* methods */
void Wish::parseMessages(const string &messages) {
	if (messages.find(MESSAGE_FOR_WHAT_DO_YOU_WISH, 0) != string::npos) {
		/* we actually got a wish?
		 * yay, well, just wish for a gdsm for the time being */
		setCommand(0, PRIORITY_CONTINUE_ACTION, wishes[0]);
		sequence = 0;
		return;
	}
}
