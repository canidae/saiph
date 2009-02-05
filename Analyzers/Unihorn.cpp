#include "Unihorn.h"
#include "../Request.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Unihorn::Unihorn(Saiph *saiph) : Analyzer("Unihorn"), saiph(saiph), unihorn_key(0), apply_priority(-1) {
}

/* methods */
void Unihorn::analyze() {
	if (apply_priority >= 0) {
		findUnihorn();
		/* we can't apply anything if we're overtaxed.
		   TODO drop things to be able to use unihorn */
		if (unihorn_key == 0 || saiph->world->player.encumbrance >= OVERTAXED) {
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
	} else if (saiph->inventory_changed) {
		findUnihorn();
	}
}

bool Unihorn::request(const Request &request) {
	if (request.request == REQUEST_APPLY_UNIHORN) {
		findUnihorn();
		if (unihorn_key == 0)
			return false;
		/* we can't apply anything if we're overtaxed.
		   TODO drop things to be able to use unihorn */
		if (saiph->world->player.encumbrance >= OVERTAXED)
			return false;
		/* we got a unicorn horn */
		if (request.priority > apply_priority)
			apply_priority = request.priority;
		command = APPLY;
		priority = apply_priority;
		return true;
	}
	return false;
}

/* private methods */
void Unihorn::findUnihorn() {
	map<unsigned char, Item>::iterator u = saiph->inventory.find(unihorn_key);
	if (u != saiph->inventory.end() && u->second.beatitude != CURSED && u->second.beatitude != BEATITUDE_UNKNOWN && u->second.name == "unicorn horn")
		return;
	for (u = saiph->inventory.begin(); u != saiph->inventory.end(); ++u) {
		if (u->second.beatitude == CURSED || u->second.name != "unicorn horn") {
			continue;
		} else if (u->second.beatitude == BEATITUDE_UNKNOWN) {
			/* don't know beatitude of unihorn, request it beatified */
			req.request = REQUEST_BEATIFY_ITEMS;
			saiph->request(req);
			continue; // don't apply unicorn with unknown beatitude either
		}
		/* this should be a unihorn */
		unihorn_key = u->first;
		return;
	}
	/* no unihorn */
	unihorn_key = 0;
}
