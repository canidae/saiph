#include "Excalibur.h"

/* constructors */
Excalibur::Excalibur(Saiph *saiph) : Analyzer("Excalibur"), saiph(saiph), excalibur_exists(false), action(""), action2("") {
}

/* methods */
void Excalibur::command(string *command) {
	*command = action;
	if (action2 == "")
		action = "";
}

void Excalibur::finish() {
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
		action = DIP;
		action2 = got_long_sword;
		priority = EXCALIBUR_DIP_PRIORITY;
	} else {
		/* no, request that someone takes us there */
		req.request = REQUEST_VISIT_NEAREST_FOUNTAIN;
		req.priority = EXCALIBUR_DIP_PRIORITY;
		saiph->request(req);
	}
}

void Excalibur::parseMessages(const string &messages) {
	if (action != "" && action2 != "" && messages.find(MESSAGE_WHAT_TO_DIP, 0) != string::npos) {
		/* what to dip... the long sword ofcourse! */
		action = action2;
		action2 = YES;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (action != "" && action2 != "" && messages.find(MESSAGE_DIP_IN_FOUNTAIN, 0) != string::npos) {
		/* if we want to dip in fountain? sure */
		action = action2;
		action2 = "";
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (messages.find(MESSAGE_RECEIVED_EXCALIBUR, 0) != string::npos) {
		/* alright! */
		excalibur_exists = true;
	}
}
