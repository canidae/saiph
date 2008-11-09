#include "Excalibur.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Excalibur::Excalibur(Saiph *saiph) : Analyzer("Excalibur"), saiph(saiph), excalibur_exists(false) {
}

/* methods */
void Excalibur::analyze() {
	if (excalibur_exists || saiph->world->player.experience < 5)
		return;
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
	unsigned char move = saiph->shortestPath(FOUNTAIN, false, &moves);
	if (move == ILLEGAL_MOVE)
		return; // don't know of any fountains
	if (move == MOVE_NOWHERE) {
		/* standing on (in?) fountain, dip */
		setCommand(0, EXCALIBUR_DIP_PRIORITY, DIP);
		setCommand(1, PRIORITY_CONTINUE_ACTION, string(1, got_long_sword));
		setCommand(2, PRIORITY_CONTINUE_ACTION, YES); // yes, dip in fountain
		sequence = 0;
	} else {
		/* move towards fountain */
		setCommand(0, EXCALIBUR_DIP_PRIORITY, string(1, move));
		sequence = 0;
	}
}

void Excalibur::parseMessages(const string &messages) {
	if (excalibur_exists)
		return;
	if (sequence == 0 && messages.find(MESSAGE_WHAT_TO_DIP, 0) != string::npos) {
		/* what to dip... the long sword ofcourse! */
		++sequence;
	} else if (sequence == 1 && messages.find(MESSAGE_DIP_IN_FOUNTAIN, 0) != string::npos) {
		/* if we want to dip in fountain? sure */
		++sequence;
	} else if (messages.find(MESSAGE_RECEIVED_EXCALIBUR, 0) != string::npos) {
		/* alright! */
		excalibur_exists = true;
		sequence = -1;
	} else if (sequence > 0) {
		sequence = -1;
	}
}
