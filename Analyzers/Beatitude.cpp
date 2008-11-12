#include "Beatitude.h"
#include "../Globals.h"
#include "../Saiph.h"

using namespace std;

/* constructors/destructor */
Beatitude::Beatitude(Saiph *saiph) : Analyzer("Beatitude"), saiph(saiph) {
}

/* methods */
void Beatitude::analyze() {
	/* how many of our items needs to be checked? */
	int items_to_beatify = 0;
	for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
		if (i->second.beatitude == BEATITUDE_UNKNOWN)
			++items_to_beatify;
	}
	if (items_to_beatify < BEATITUDE_DROP_ALTAR_MIN)
		return; // need more items before we head for an altar
	int moves = 0;
	unsigned char move = saiph->shortestPath(ALTAR, false, &moves);
	if (move == ILLEGAL_MOVE)
		return; // don't know of any altars
	if (move == MOVE_NOWHERE) {
		/* we're standing on the altar, drop items */
		setCommand(0, BEATITUDE_DROP_ALTAR_PRIORITY, DROP);
		sequence = 0;
	} else {
		items_to_beatify -= moves * BEATITUDE_DROP_ALTAR_ADD_PER_1000_MOVE / 1000;
		if (items_to_beatify < BEATITUDE_DROP_ALTAR_MIN)
			return; // too far to nearest altar
		/* moving towards altar */
		setCommand(0, BEATITUDE_DROP_ALTAR_PRIORITY, string(1, move));
		sequence = 0;
	}
}

void Beatitude::parseMessages(const string &messages) {
	if (saiph->got_drop_menu && saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] == ALTAR) {
		/* drop stuff we don't know beatitude of */
		for (map<unsigned char, Item>::iterator d = saiph->drop.begin(); d != saiph->drop.end(); ++d) {
			if (d->second.beatitude != BEATITUDE_UNKNOWN)
				continue;
			if (d->second.name == "gold piece")
				continue; // don't drop gold
			/* mark this */
			setCommand(0, PRIORITY_SELECT_ITEM, string(1, d->first));
			sequence = 0;
			return;
		}
		/* if we got this far, we've selected everything we don't know beatitude of (on this page) */
		setCommand(0, PRIORITY_CLOSE_PAGE, CLOSE_PAGE);
		sequence = 0;
	}
}
