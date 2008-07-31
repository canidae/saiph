#include "Beatitude.h"

/* constructors */
Beatitude::Beatitude(Saiph *saiph) : Analyzer("Beatitude"), saiph(saiph), check_beatitude(false) {
}

/* methods */
void Beatitude::begin() {
	if (!saiph->world->menu && got_drop_menu) {
		check_beatitude = false;
		got_drop_menu = false;
		/* we should look on the ground so we'll pick up the stuff we just dropped,
		 * if we still want it, that is */
		command = LOOK;
		priority = PRIORITY_LOOK;
		return;
	}
	if (got_drop_menu || saiph->best_priority > BEATITUDE_DROP_ALTAR_PRIORITY)
		return;
	unsigned char move = ILLEGAL_MOVE;
	if (!check_beatitude && saiph->world->player.turn % 100 == 0) {
		/* how many of our items needs to be checked? */
		int items_to_beatify = 0;
		for(map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (i->second.beatitude == BEATITUDE_UNKNOWN)
				++items_to_beatify;
		}
		if (items_to_beatify >= BEATITUDE_DROP_ALTAR_MIN) {
			int moves = 0;
			move = saiph->shortestPath(ALTAR, false, &moves);
			if (move == ILLEGAL_MOVE)
				return; // don't know of any altars
			items_to_beatify -= moves * BEATITUDE_DROP_ALTAR_ADD_PER_1000_MOVE / 1000;
			if (items_to_beatify >= BEATITUDE_DROP_ALTAR_MIN)
				check_beatitude = true;
		}
	}
	if (!check_beatitude)
		return;
	/* are we standing on an altar */
	if (saiph->dungeonmap[saiph->position.branch][saiph->position.level][saiph->position.row][saiph->position.col] == ALTAR) {
		/* yes, drop items with unknown beatitude */
		command = DROP;
		priority = BEATITUDE_DROP_ALTAR_PRIORITY;
	} else {
		/* no, path to nearest altar */
		if (move == ILLEGAL_MOVE) {
			int moves = 0;
			move = saiph->shortestPath(ALTAR, false, &moves);
		}
		if (move == ILLEGAL_MOVE)
			return; // don't know of any altars
		command = move;
		priority = BEATITUDE_DROP_ALTAR_PRIORITY;
	}
}

void Beatitude::parseMessages(const string &messages) {
	if (!check_beatitude || !saiph->world->menu)
		return;
	if (check_beatitude && command.size() == 1 && command[0] == DROP && messages.find(BEATITUDE_DROP_WHAT_TYPE, 0) != string::npos) {
		command = 'a';
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (check_beatitude && messages.find(BEATITUDE_DROP_WHICH_ITEMS, 0) != string::npos) {
		got_drop_menu = true;
	}
	if (got_drop_menu) {
		/* drop stuff we don't know beatitude of */
		for(map<unsigned char, Item>::iterator d = saiph->drop.begin(); d != saiph->drop.end(); ++d) {
			if (d->second.beatitude == BEATITUDE_UNKNOWN) {
				/* mark this */
				command = d->first;
				priority = PRIORITY_SELECT_ITEM;
				return;
			}
		}
		/* if we got this far, we've selected everything we don't know beatitude of (on this page) */
		command = " ";
		priority = PRIORITY_CLOSE_ITEM_LIST;
	}
}
