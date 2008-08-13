#include "Unihorn.h"

/* constructors */
Unihorn::Unihorn(Saiph *saiph) : Analyzer("Unihorn"), saiph(saiph), unihorn_key(0), apply_priority(-1) {
}

/* methods */
void Unihorn::analyze() {
	if (apply_priority >= 0 && unihorn_key != 0) {
		/* unihorn failed last attempt, try again */
		command = APPLY;
		priority = apply_priority;
	}
}

void Unihorn::parseMessages(const string &messages) {
	if (saiph->world->question && messages.find(MESSAGE_WHAT_TO_APPLY, 0) != string::npos) {
		command = unihorn_key;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (messages.find(UNIHORN_NOTHING_HAPPENS, 0) != string::npos) {
		apply_priority = -1;
	}
}

bool Unihorn::request(const Request &request) {
	if (request.request == REQUEST_APPLY_UNIHORN) {
		if (unihorn_key == 0 || saiph->inventory[unihorn_key].beatitude == CURSED || saiph->inventory[unihorn_key].name != "unicorn horn") {
			for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
				if (i->second.beatitude == CURSED || i->second.name != "unicorn horn")
					continue;
				unihorn_key = i->first;
				break;
			}
		}
		if (unihorn_key == 0)
			return false;
		/* we got a unicorn horn */
		if (request.priority < apply_priority)
			return true;
		command = APPLY;
		priority = request.priority;
		apply_priority = priority;
	}
	return false;
}
