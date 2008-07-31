#include "Beatitude.h"

/* constructors */
Beatitude::Beatitude(Saiph *saiph) : Analyzer("Beatitude"), saiph(saiph), check_beatitude(false) {
}

/* methods */
void Beatitude::begin() {
	if (saiph->best_priority > BEATITUDE_DROP_ALTAR_PRIORITY)
		return;
	if (!check_beatitude && saiph->world->player.turn % 100 == 0) {
		/* how many of our items needs to be checked? */
		int items_to_beatify = 0;
		for(map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (i->second.beatitude == BEATITUDE_UNKNOWN)
				++items_to_beatify;
		}
		if (items_to_beatify >= BEATITUDE_DROP_ALTAR_MIN)
			check_beatitude = true;
	}
	if (!check_beatitude)
		return;
	/* are we standing on an altar */
	if (saiph->dungeonmap[saiph->position.branch][saiph->position.level][saiph->position.row][saiph->position.col] == FOUNTAIN) {
		/* yes, drop items with unknown beatitude */
		command = DROP;
		priority = BEATITUDE_DROP_ALTAR_PRIORITY;
	} else {
		/* no, path to nearest altar */
		int moves = 0;
		unsigned char move = saiph->shortestPath(ALTAR, false, &moves);
		if (move == ILLEGAL_MOVE)
			return; // don't know of any altars
		command = move;
		priority = BEATITUDE_DROP_ALTAR_PRIORITY;
	}
}

void Beatitude::parseMessages(const string &messages) {
	if (!beatitude || !saiph->world->menu)
		return;
	if (command.size() == 1 && command[0] == DROP && messages.find(BEATITUDE_DROP_WHAT_TYPE, 0) != string::npos) {
		command = 'a';
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (command.size() == 1 && command[0] == 'a' && messages.find(BEATITUDE_DROP_WHICH_ITEMS, 0) != string::npos) {
		got_drop_menu = true;
	}
	if (got_drop_menu) {
	}
}
