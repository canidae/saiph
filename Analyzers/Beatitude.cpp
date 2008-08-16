#include "Beatitude.h"

/* constructors */
Beatitude::Beatitude(Saiph *saiph) : Analyzer("Beatitude"), saiph(saiph), check_beatitude(false), got_drop_menu(false) {
}

/* methods */
void Beatitude::analyze() {
	if (command == LOOK) {
		/* still not looked at ground? */
		priority = PRIORITY_LOOK;
		return;
	}
	if (got_drop_menu || saiph->best_priority > BEATITUDE_DROP_ALTAR_PRIORITY)
		return;
	unsigned char move = ILLEGAL_MOVE;
	if (!check_beatitude && saiph->inventory_changed) {
		/* how many of our items needs to be checked? */
		int items_to_beatify = 0;
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
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
	/* path to nearest altar (if we haven't already) */
	if (move == ILLEGAL_MOVE) {
		int moves = 0;
		move = saiph->shortestPath(ALTAR, false, &moves);
	}
	if (move == ILLEGAL_MOVE)
		return; // don't know of any altars
	if (move == MOVE_NOWHERE) {
		/* we're standing on the altar, drop items */
		command = DROP;
		priority = BEATITUDE_DROP_ALTAR_PRIORITY;
	} else {
		/* moving towards altar */
		command = move;
		priority = BEATITUDE_DROP_ALTAR_PRIORITY;
	}
}

void Beatitude::complete() {
	if (command == LOOK)
		command.clear();
}

void Beatitude::parseMessages(const string &messages) {
	if (!saiph->world->menu && got_drop_menu) {
		/* no longer got drop menu */
		check_beatitude = false;
		got_drop_menu = false;
		/* we should look on the ground so we'll pick up the stuff we just dropped,
		 * if we still want it, that is */
		command = LOOK;
		priority = PRIORITY_LOOK;
		/* and request a dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
		return;
	}
	if (!check_beatitude || !saiph->world->menu)
		return;
	if (check_beatitude && saiph->drop.size() > 0 && saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] == ALTAR)
		got_drop_menu = true;
	if (got_drop_menu) {
		/* drop stuff we don't know beatitude of */
		for (map<unsigned char, Item>::iterator d = saiph->drop.begin(); d != saiph->drop.end(); ++d) {
			if (d->second.beatitude != BEATITUDE_UNKNOWN)
				continue;
			if (d->second.name == "gold piece")
				continue; // don't drop gold
			/* mark this */
			command = d->first;
			priority = PRIORITY_SELECT_ITEM;
			return;
		}
		/* if we got this far, we've selected everything we don't know beatitude of (on this page) */
		command = CLOSE_PAGE;
		priority = PRIORITY_CLOSE_ITEM_LIST;
	}
}
