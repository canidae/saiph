#include "Excalibur.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Excalibur::Excalibur(Saiph *saiph) : Analyzer("Excalibur"), saiph(saiph), command2("") {
}

/* methods */
void Excalibur::analyze() {
	if (saiph->world->player.experience < 5)
		return;
	else if (saiph->world->player.blind)
		return; // don't move when blind
	/* do we have a long sword? */
	unsigned char got_long_sword = ILLEGAL_ITEM;
	for(map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		if (i->second.name == EXCALIBUR_LONG_SWORD) {
			got_long_sword = i->first;
			break;
		}
	}
	if (got_long_sword == ILLEGAL_ITEM)
		return;
	/* path to nearest fountain */
	int moves = 0;
	unsigned char dir = saiph->shortestPath(FOUNTAIN, false, &moves);
	if (dir == ILLEGAL_DIRECTION)
		return; // don't know of any fountains
	if (dir == NOWHERE) {
		/* standing on (in?) fountain, dip */
		command = DIP;
		command2 = got_long_sword;
		priority = PRIORITY_EXCALIBUR_DIP;
	} else {
		/* move towards fountain */
		command = dir;
		command2.clear();
		priority = PRIORITY_EXCALIBUR_DIP;
	}
}

void Excalibur::parseMessages(const string &messages) {
	if (command != "" && command2 != "" && messages.find(MESSAGE_WHAT_TO_DIP, 0) != string::npos) {
		/* what to dip... the long sword ofcourse! */
		command = command2;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (command != "" && command2 != "" && messages.find(MESSAGE_DIP_IN_FOUNTAIN, 0) != string::npos) {
		/* if we want to dip in fountain? sure */
		command = YES;
		command2.clear();
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (messages.find(MESSAGE_RECEIVED_EXCALIBUR, 0) != string::npos) {
		/* alright! */
		expired = true; // delete this analyzer from analyzers
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	}
}
