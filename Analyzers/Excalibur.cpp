#include "Excalibur.h"

/* constructors */
Excalibur::Excalibur(Saiph *saiph) : Analyzer("Excalibur"), saiph(saiph), excalibur_exists(false), command2("") {
}

/* methods */
void Excalibur::complete() {
	if (command2 == "")
		command = "";
}

void Excalibur::finish() {
	if (saiph->best_priority > EXCALIBUR_DIP_PRIORITY)
		return;
	if (excalibur_exists)
		return;
	if (saiph->world->player.experience < 5)
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
	/* are we standing on a fountain? */
	if (saiph->dungeonmap[saiph->position.branch][saiph->position.level][saiph->position.row][saiph->position.col] == FOUNTAIN) {
		/* yes, dip */
		command = DIP;
		command2 = got_long_sword;
		priority = EXCALIBUR_DIP_PRIORITY;
	} else {
		/* no, path to nearest fountain */
		int moves = 0;
		unsigned char move = saiph->shortestPath(FOUNTAIN, false, &moves);
		if (move == ILLEGAL_MOVE)
			return; // don't know of any fountains
		command = move;
		priority = EXCALIBUR_DIP_PRIORITY;
	}
}

void Excalibur::parseMessages(const string &messages) {
	if (excalibur_exists)
		return;
	if (command != "" && command2 != "" && messages.find(MESSAGE_WHAT_TO_DIP, 0) != string::npos) {
		/* what to dip... the long sword ofcourse! */
		command = command2;
		command2 = YES;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (command != "" && command2 != "" && messages.find(MESSAGE_DIP_IN_FOUNTAIN, 0) != string::npos) {
		/* if we want to dip in fountain? sure */
		command = command2;
		command2 = "";
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (messages.find(MESSAGE_RECEIVED_EXCALIBUR, 0) != string::npos) {
		/* alright! */
		excalibur_exists = true;
	}
}
