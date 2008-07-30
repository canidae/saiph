#include "Enhance.h"

/* constructors */
Enhance::Enhance(Saiph *saiph) : Analyzer("Enhance"), saiph(saiph), do_enhance(true), got_enhance_menu(false) {
}

/* methods */
void Enhance::parseMessages(const string &messages) {
	if (messages.find(ENHANCE_MESSAGE_CONFIDENT, 0) != string::npos) {
		do_enhance = true;
		return;
	} else if (messages.find(ENHANCE_CURRENT_SKILLS, 0) != string::npos) {
		got_enhance_menu = true;
	}
	if (got_enhance_menu && !saiph->world->menu) {
		/* no longer showing a menu, make those booleans false */
		do_enhance = false;
		got_enhance_menu = false;
		return;
	} else if (do_enhance && !saiph->world->menu) {
		/* request that the enhance menu is shown */
		command = ENHANCE_ENHANCE;
		priority = PRIORITY_LOOK;
		return;
	} else if (got_enhance_menu && saiph->world->menu) {
		/* showing enhance menu.
		 * enhance 'a' for now */
		command = "a";
		priority = PRIORITY_CONTINUE_ACTION;
		return;
	}
}
