#include "Beatitude.h"
#include "../Globals.h"
#include "../Request.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Beatitude::Beatitude(Saiph *saiph) : Analyzer("Beatitude"), saiph(saiph), check_beatitude(false) {
}

/* methods */
void Beatitude::analyze() {
	/* TODO: this should go, other analyzers will have to request beatifying items */
	if (saiph->world->player.blind)
		return; // no buc-testing while blind

	unsigned char dir = ILLEGAL_DIRECTION;
	int moves = 0;
	if (!check_beatitude && saiph->inventory_changed) {
		/* how many of our items needs to be checked? */
		int items_to_beatify = 0;
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (beatify(i->second))
				++items_to_beatify;
		}
		if (items_to_beatify >= BEATITUDE_DROP_ALTAR_MIN) {
			dir = saiph->shortestPath(ALTAR, true, &moves);
			if (dir == ILLEGAL_DIRECTION)
				return; // don't know of any altars
			items_to_beatify -= moves * BEATITUDE_DROP_ALTAR_ADD_PER_1000_MOVE / 1000;
			if (items_to_beatify >= BEATITUDE_DROP_ALTAR_MIN)
				check_beatitude = true;
		}
	}
	if (!check_beatitude)
		return;
	/* path to nearest altar (if we haven't already) */
	if (dir == ILLEGAL_DIRECTION)
		dir = saiph->shortestPath(ALTAR, true, &moves);
	if (dir == ILLEGAL_DIRECTION)
		return; // don't know of any altars
	if (dir == NOWHERE) {
		/* we're standing on the altar, drop items */
		command = DROP;
		priority = BEATITUDE_DROP_ALTAR_PRIORITY;
	} else {
		/* move towards altar */
		command = dir;
		priority = BEATITUDE_DROP_ALTAR_PRIORITY;
	}
}

void Beatitude::parseMessages(const string &messages) {
	if (saiph->got_drop_menu && saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] == ALTAR) {
		/* drop stuff we don't know beatitude of */
		for (map<unsigned char, Item>::iterator d = saiph->drop.begin(); d != saiph->drop.end(); ++d) {
			if (!beatify(d->second))
				continue;
			/* mark this */
			command = d->first;
			priority = PRIORITY_SELECT_ITEM;
			return;
		}
		/* if we got this far, we've selected everything we don't know beatitude of (on this page) */
		check_beatitude = false;
		command = CLOSE_PAGE;
		priority = PRIORITY_CLOSE_PAGE;
	}
}

bool Beatitude::request(const Request &request) {
	if (request.request == REQUEST_BEATIFY_ITEMS) {
		check_beatitude = true;
		return true;
	}
	return false;
}

/* private methods */
bool Beatitude::beatify(const Item &item) {
	if (item.beatitude != BEATITUDE_UNKNOWN)
		return false;
	if (item.name == "gold piece")
		return false;
	if (item.additional == "being worn")
		return false;
	if (item.additional == "embedded in your skin")
		return false;
	if (item.additional == "in use")
		return false;
	if (item.additional == "wielded")
		return false;
	if (item.additional.find("weapon in ", 0) == 0 || item.additional.find("wielded in other ", 0) == 0)
		return false;
	if (item.additional.find("on left ", 0) == 0 || item.additional.find("on right ", 0) == 0)
		return false;
	return true;
}
