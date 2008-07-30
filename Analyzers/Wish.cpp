#include "Wish.h"

/* constructors */
Wish::Wish(Saiph *saiph) : Analyzer("Wish"), saiph(saiph) {
	wishes.push_back("blessed greased fixed +3 gray dragon scale mail");
}

/* methods */
void Wish::parseMessages(const string &messages) {
	if (messages.find(WISH_MESSAGE, 0) != string::npos) {
		/* we actually got a wish?
		 * yay, well, just wish for a gdsm for the time being */
		command = *(wishes.begin());
		command.append("\n");
		priority = PRIORITY_CONTINUE_ACTION;
		return;
	}
}
