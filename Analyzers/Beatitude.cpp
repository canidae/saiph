#include "Beatitude.h"
#include "../Globals.h"
#include "../Items.h"
#include "../Request.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Beatitude::Beatitude(Saiph *saiph) : Analyzer("Beatitude"), saiph(saiph), check_beatitude(false) {
}

/* methods */
void Beatitude::analyze() {
	if (!check_beatitude)
		return; // noone has requested beatifying items
	else if (priority >= PRIORITY_BEATITUDE_DROP_ALTAR)
		return; // got something more important to do
	else if (saiph->world->player.blind)
		return; // no buc-testing while blind

	/* path to nearest altar */
	int moves = 0;
	unsigned char dir = saiph->shortestPath(ALTAR, false, &moves);
	if (dir == ILLEGAL_DIRECTION)
		return; // don't know of any altars
	else if (dir == NOWHERE)
		command = DROP_MENU; // we're standing on the altar, drop items
	else
		command = dir; // move towards altar
	priority = PRIORITY_BEATITUDE_DROP_ALTAR;
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
