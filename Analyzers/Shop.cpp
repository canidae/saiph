#include <stdlib.h>
#include "Shop.h"
#include "../Debug.h"
#include "../Globals.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Shop::Shop(Saiph *saiph) : Analyzer("Shop"), saiph(saiph), direction(ILLEGAL_DIRECTION), state(SHOP_IDLE) {
}

/* methods */
void Shop::parseMessages(const string &messages) {
	if (messages.find(SHOP_MESSAGE_PICKAXE, 0) != string::npos || messages.find(SHOP_MESSAGE_PICKAXE_PLURAL, 0) != string::npos || messages.find(SHOP_MESSAGE_MATTOCK, 0) != string::npos || messages.find(SHOP_MESSAGE_MATTOCK_PLURAL, 0) != string::npos || messages.find(SHOP_MESSAGE_TOOLS, 0) != string::npos || messages.find(SHOP_MESSAGE_ANGRY_PICKAXE, 0) != string::npos || messages.find(SHOP_MESSAGE_ANGRY_PICKAXE_PLURAL, 0) != string::npos || messages.find(SHOP_MESSAGE_ANGRY_MATTOCK, 0) != string::npos || messages.find(SHOP_MESSAGE_ANGRY_MATTOCK_PLURAL, 0) != string::npos || messages.find(SHOP_MESSAGE_ANGRY_TOOLS, 0) != string::npos) {
		/* Find where the shopkeeper is standing. He should be right next to us blocking the door. */
		Point sk_pos;
		for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
			if (!m->second.shopkeeper || !m->second.visible)
				continue;

			int distance = max(abs(m->first.row - saiph->position.row), abs(m->first.col - saiph->position.col));
			if (distance == 1) {
				/* Hopefully there are no places where two shopkeepers will be next to us. */
				sk_pos = m->first;
				break;
			}
		}

		exit = saiph->position;

		if (sk_pos.row == saiph->position.row) {
			if (sk_pos.col > saiph->position.col) {
				direction = E;
				command = W;
			} else {
				direction = W;
				command = E;
			}
		} else {
			if (sk_pos.row > saiph->position.row) {
				direction = N;
				command = S;
			} else {
				direction = S;
				command = N;
			}
		}
		priority = PRIORITY_CONTINUE_ACTION;

		Debug::notice() << "[Shop       ] moving in direction " << command << endl;
		state = SHOP_MOVE_AWAY;
	} else if ((state == SHOP_MOVE_AWAY) && (exit != saiph->position)) {

		/* shopkeeper asking us to drop our tools */
		command = DROP;
		priority = PRIORITY_CONTINUE_ACTION;

		state = SHOP_DROP_TOOLS;
		Debug::notice() << "[Shop       ] opening drop menu" << endl;
	} else if (saiph->got_drop_menu && (state != SHOP_IDLE)) {
		/* drop our tools */
		for (map<unsigned char, Item>::iterator d = saiph->drop.begin(); d != saiph->drop.end(); ++d) {
			if (d->second.name != "pick-axe" && d->second.name != "mattock")
				continue;
			command = d->first;
			priority = PRIORITY_SELECT_ITEM;
			Debug::notice() << "[Shop       ] trying to drop digging tool: " << command << endl;
			return;
		}
		/* we've probably selected our gold if we're here */
		command = CLOSE_PAGE;
		priority = PRIORITY_CLOSE_PAGE;

		state = SHOP_LOOK_AT_GROUND;
		Debug::notice() << "[Shop       ] closing drop menu" << endl;
	} else if (state == SHOP_LOOK_AT_GROUND) {
		/* we'll look at ground after dropping the tools.
		 * this makes us aware of the stash,
		 * and the loot analyzer won't "visit" the stash after we move */
		command = LOOK;
		priority = PRIORITY_LOOK;

		state = SHOP_MOVE_INTO_SHOP;
		Debug::notice() << "[Shop       ] looking at the ground" << endl;
	} else if (state == SHOP_MOVE_INTO_SHOP) {
		command = direction;
		priority = PRIORITY_SHOP_ENTER;

		Debug::notice() << "[Shop       ] moving direction " << direction << endl;

		/* If we're at the exit, mark us as browsing since we're entering this turn */
		// Should this be set in the analyze step? */
		if (exit == saiph->position)
			state = SHOP_BROWSING;
	}
}

void Shop::analyze() {
	if (saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] != FLOOR)
		return; // not standing on FLOOR, not in shop or shop already detected
	bool inside_shop = false;

	for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
		if (!m->second.shopkeeper || !m->second.visible)
			continue;

		/* Figure out of we're in the same room as the shopkeeper */
		int top = m->first.row;
		int bottom = m->first.row;
		int left = m->first.col;
		int right = m->first.col;

		unsigned char symbol = 0;

		symbol = saiph->levels[saiph->position.level].dungeonmap[--top][m->first.col];
		while ((symbol != HORIZONTAL_WALL) && (symbol != OPEN_DOOR) && (top >= 0))
			symbol = saiph->levels[saiph->position.level].dungeonmap[--top][m->first.col];

		symbol = saiph->levels[saiph->position.level].dungeonmap[++bottom][m->first.col];
		while ((symbol != HORIZONTAL_WALL) && (symbol != OPEN_DOOR) && (bottom <= MAP_ROW_END))
			symbol = saiph->levels[saiph->position.level].dungeonmap[++bottom][m->first.col];

		symbol = saiph->levels[saiph->position.level].dungeonmap[m->first.row][--left];
		while ((symbol != VERTICAL_WALL) && (symbol != OPEN_DOOR) && (left >= 0))
			symbol = saiph->levels[saiph->position.level].dungeonmap[m->first.row][--left];

		symbol = saiph->levels[saiph->position.level].dungeonmap[m->first.row][++right];
		while ((symbol != VERTICAL_WALL) && (symbol != OPEN_DOOR) && (right <= MAP_COL_END))
			symbol = saiph->levels[saiph->position.level].dungeonmap[m->first.row][++right];

		if (saiph->position.row > top && saiph->position.row < bottom && saiph->position.col > left && saiph->position.col < right)
			inside_shop = true;

		Debug::notice() << "[Shop       ] bounds are (" << top << ", " << left << ", " << bottom << ", " << right << "), inside? " << inside_shop << endl;
	}

	saiph->world->player.inside_shop = inside_shop;

	if (saiph->world->player.inside_shop && exit == saiph->position && state == SHOP_BROWSING)
		state = SHOP_IDLE;
}
