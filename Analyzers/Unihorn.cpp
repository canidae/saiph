#include "Unihorn.h"

/* constructors */
Unihorn::Unihorn(Saiph *saiph) : Analyzer("Unihorn"), saiph(saiph), unihorn_key(0), apply_priority(-1) {
}

/* methods */
void Unihorn::analyze() {
	if (apply_priority >= 0) {
		findUnihorn();
		if (unihorn_key == 0) {
			/* lost unihorn somehow */
			apply_priority = -1;
			return;
		}
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
		findUnihorn();
		if (unihorn_key == 0)
			return false;
		/* we got a unicorn horn */
		command = APPLY;
		if (request.priority > apply_priority)
			priority = request.priority;
		apply_priority = priority;
		return true;
	}
	return false;
}

/* private methods */
void Unihorn::findUnihorn() {
	map<unsigned char, Item>::iterator u = saiph->inventory.find(unihorn_key);
	if (u != saiph->inventory.end() && u->second.beatitude != CURSED && u->second.name == "unicorn horn")
		return;
	for (u = saiph->inventory.begin(); u != saiph->inventory.end(); ++u) {
		if (u->second.beatitude == CURSED || u->second.name != "unicorn horn")
			continue;
		/* this should be a unihorn */
		unihorn_key = u->first;
		return;
	}
	/* no unihorn */
	unihorn_key = 0;
}
