#include "Elbereth.h"

/* constructors */
Elbereth::Elbereth(Saiph *saiph) : Analyzer("Elbereth"), saiph(saiph) {
	action = "";
	elbereth_count = 0;
	burned = false;
	digged = false;
	dusted = false;
	did_look = false;
	append = false;
	priority = ILLEGAL_PRIORITY;
}

/* methods */
void Elbereth::command(string *command) {
	*command = action;
}

void Elbereth::parseMessages(const string &messages) {
	/* reset these each time */
	dusted = false;
	burned = false;
	digged = false;
	elbereth_count = 0;
	if (action == ":") {
		did_look = true;
		action = "";
	} else {
		did_look = false;
	}
	/* set up next command */
	if (action.size() > 0 && action[0] == ENGRAVE && messages.find(MESSAGE_ENGRAVE_WITH, 0) != string::npos) {
		priority = PRIORITY_CONTINUE_ACTION;
		action = HANDS;
	} else if (action.size() > 0 && action[0] == HANDS && messages.find(MESSAGE_ENGRAVE_ADD, 0) != string::npos) {
		priority = PRIORITY_CONTINUE_ACTION;
		action = append ? YES : NO;
	} else if (action.size() > 0 && (action[0] == YES || action[0] == NO || action[0] == HANDS) && (messages.find(MESSAGE_ENGRAVE_DUST_ADD, 0) != string::npos || messages.find(MESSAGE_ENGRAVE_DUST, 0) != string::npos)) {
		priority = PRIORITY_CONTINUE_ACTION;
		action = ELBERETH "\n";
	}
	/* figure out if something is engraved here */
	string::size_type pos = messages.find(MESSAGE_YOU_READ, 0);
	if (pos == string::npos) {
		/* no elbereths here :( */
		return;
	}
	/* is it written in dust, burned or digged? */
	if (messages.find(MESSAGE_DUSTED_TEXT, 0) != string::npos) {
		/* it's dusted. */
		dusted = true;
	} else if (messages.find(MESSAGE_BURNED_TEXT, 0) != string::npos) {
		/* it's burned */
		burned = true;
	} else if (messages.find(MESSAGE_DIGGED_TEXT, 0) != string::npos) {
		/* it's digged */
		digged = true;
	} else {
		/* it's unexpected */
		return;
	}
	while ((pos = messages.find(ELBERETH, pos + 1)) != string::npos) {
		/* found another elbereth */
		++elbereth_count;
	}
}

bool Elbereth::request(const Request &request) {
	if (request.request == REQUEST_ELBERETH_OR_REST) {
		/* check for ill effects */
		if (saiph->world->player.blind || saiph->world->player.confused || saiph->world->player.hallucinating || saiph->world->player.stunned)
			return false;
		/* check that we're not engulfed */
		if (saiph->engulfed)
			return false;
		/* check that the monsters around us respects elbereth */
		for (map<Point, Monster>::iterator m = saiph->monstertracker->monsters[saiph->position.branch][saiph->position.level].begin(); m!= saiph->monstertracker->monsters[saiph->position.branch][saiph->position.level].end(); ++m) {
			if (abs(saiph->position.row - m->first.row) > 1 || abs(saiph->position.col - m->first.col) > 1) {
				/* monster is not next to player */
				continue;
			}
			/* TODO
			 * monster that won't respect elbereth might be friendly.
			 * we also need to check for minotaurs & riders (heh, yeah, right) */
			if (m->second.symbol == '@')
				return false; // elbereth won't be respected
			if (m->second.symbol == 'A')
				return false; // elbereth won't be respected
		}
		if (!did_look && !burned && !digged && !dusted) {
			/* we'll need to look first, which means set action & priority and return true */
			action = ":";
			priority = PRIORITY_LOOK; // since it's a zero turn affair
			return true;
		} else {
			/* we know what's here */
			if (((burned || digged) && elbereth_count > 0) || (dusted && elbereth_count >= 3)) {
				/* we should rest */
				action = "20.";
				priority = request.priority;
				return true;
			} else if (!burned && !digged && elbereth_count < 3) {
				/* we should engrave in the dust */
				append = (elbereth_count > 0); // append if 0 < elbereth_count < 3
				action = ENGRAVE;
				priority = request.priority;
				return true;
			} else {
				/* hmm... we don't know how to handle digged elbereths that fades out */
				return false;
			}
		}
	}
	return false;
}
