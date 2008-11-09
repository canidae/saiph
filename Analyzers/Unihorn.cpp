#include "Unihorn.h"
#include "../Request.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Unihorn::Unihorn(Saiph *saiph) : Analyzer("Unihorn"), saiph(saiph), unihorn_key(0) {
}

/* methods */
void Unihorn::complete() {
	if (sequence == 0)
		++sequence;
}

void Unihorn::parseMessages(const string &messages) {
	if (sequence == 1 && saiph->world->question && messages.find(MESSAGE_WHAT_TO_APPLY, 0) != string::npos) {
		/* back to start in case unihorn failed */
		sequence = 0;
	} else if (sequence == 0 && messages.find(MESSAGE_NOTHING_HAPPENS, 0) != string::npos) {
		/* no more bad stuff to fix */
		sequence = -1;
	}
}

bool Unihorn::request(const Request &request) {
	if (request.request == REQUEST_APPLY_UNIHORN) {
		findUnihorn();
		if (unihorn_key == 0)
			return false;
		/* we got a unicorn horn */
		setCommand(0, request.priority, APPLY);
		setCommand(1, PRIORITY_CONTINUE_ACTION, string(unihorn_key, 1));
		sequence = 0;
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
