#include <stdlib.h>
#include "Elbereth.h"
#include "../Request.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Elbereth::Elbereth(Saiph *saiph) : Analyzer("Elbereth"), saiph(saiph) {
	last_look_internal_turn = 0;
	elbereth_count = 0;
	burned = false;
	digged = false;
	dusted = false;
	frosted = false;
}

/* methods */
void Elbereth::complete() {
	if (sequence == 0 && commands[0].data == LOOK) {
		/* we're going to look now. reset variables telling us what's engraved here */
		elbereth_count = 0;
		dusted = false;
		burned = false;
		digged = false;
		frosted = false;
		last_look_internal_turn = saiph->internal_turn;
	}
}

void Elbereth::parseMessages(const string &messages) {
	/* set up next command */
	if (sequence == 0 && messages.find(MESSAGE_ENGRAVE_WITH, 0) != string::npos) {
		++sequence;
	} else if (sequence == 1 && messages.find(MESSAGE_ENGRAVE_ADD, 0) != string::npos) {
		/* we may get this message */
		++sequence;
	} else if (sequence > 0 && (messages.find(MESSAGE_ENGRAVE_DUST_ADD, 0) != string::npos || messages.find(MESSAGE_ENGRAVE_DUST, 0) != string::npos || messages.find(MESSAGE_ENGRAVE_FROST_ADD, 0) != string::npos || messages.find(MESSAGE_ENGRAVE_FROST, 0) != string::npos)) {
		/* depending on whether we got MESSAGE_ENGRAVE_ADD or not,
		 * we're on step 1 or 2.
		 * in any case, go to step 3 */
		sequence = 3;
	}
	/* figure out if something is engraved here */
	string::size_type pos = messages.find(MESSAGE_YOU_READ, 0);
	if (pos == string::npos) {
		/* no elbereths here :( */
		return;
	}
	/* is it written in dust, burned or digged? */
	if (messages.find(MESSAGE_TEXT_DUSTED, 0) != string::npos) {
		/* it's dusted. */
		dusted = true;
	} else if (messages.find(MESSAGE_TEXT_BURNED, 0) != string::npos) {
		/* it's burned */
		burned = true;
	} else if (messages.find(MESSAGE_TEXT_DIGGED, 0) != string::npos) {
		/* it's digged */
		digged = true;
	} else if (messages.find(MESSAGE_TEXT_FROSTED, 0) != string::npos) {
		/* it's frosted */
		frosted = true;
	} else {
		/* it's unexpected */
		return;
	}
	while ((pos = messages.find(ELBERETH_ELBERETH, pos + 1)) != string::npos) {
		/* found another elbereth */
		++elbereth_count;
	}
}

bool Elbereth::request(const Request &request) {
	if (request.request == REQUEST_ELBERETH_OR_REST) {
		if (!canEngrave())
			return false;
		/* we can engrave and elbereth will (probably) be respected */
		if (last_look_internal_turn != saiph->internal_turn) {
			/* we'll need to look first, which means set action & priority and return true */
			setCommand(0, PRIORITY_LOOK, LOOK, true);
			sequence = 0;
			return true;
		} else {
			/* we know what's here */
			if (((burned || digged) && elbereth_count > 0) || ((dusted || frosted) && elbereth_count >= 3)) {
				/* we should rest */
				setCommand(0, request.priority, "20" REST);
				sequence = 0;
				return true;
			} else if (!burned && !digged && elbereth_count < 3) {
				/* we should engrave in the dust/frost */
				setCommand(0, request.priority, ENGRAVE);
				setCommand(1, PRIORITY_CONTINUE_ACTION, HANDS);
				if (elbereth_count > 0)
					setCommand(2, PRIORITY_CONTINUE_ACTION, YES); // append
				else
					setCommand(2, PRIORITY_CONTINUE_ACTION, NO); // wipe out
				setCommand(3, PRIORITY_CONTINUE_ACTION, ELBERETH_ELBERETH "\n");
				sequence = 0;
				return true;
			} else {
				/* hmm... we don't know how to handle digged elbereths that fades out */
				return false;
			}
		}
	}
	return false;
}

/* private methods */
bool Elbereth::canEngrave() {
	/* TODO: check if we're levitating */
	/* check if there's a fountain/grave/altar here */
	switch (saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col]) {
		case ALTAR:
		case GRAVE:
		case FOUNTAIN:
			return false;
			break;
	}
	/* check for ill effects */
	if (saiph->world->player.blind || saiph->world->player.confused || saiph->world->player.hallucinating || saiph->world->player.stunned)
		return false;
	/* check that we're not engulfed */
	if (saiph->engulfed)
		return false;
	/* check that the monsters around us respects elbereth */
	for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m!= saiph->levels[saiph->position.level].monsters.end(); ++m) {
		if (abs(saiph->position.row - m->first.row) > 1 || abs(saiph->position.col - m->first.col) > 1) {
			/* monster is not next to player */
			continue;
		}
		if (m->second.attitude == FRIENDLY)
			continue;
		if (m->second.symbol == '@' || m->second.symbol == 'A' || m->second.symbol == 'I' || m->second.minotaur)
			return false; // elbereth won't be respected
	}
	return true;
}
