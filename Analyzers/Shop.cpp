#include <stdlib.h>
#include "Shop.h"
#include "../Debug.h"
#include "../Globals.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Shop::Shop(Saiph *saiph) : Analyzer("Shop"), saiph(saiph), drop_pick_axe(false), look_at_ground(false) {
}

/* methods */
void Shop::parseMessages(const string &messages) {
	if (messages.find(SHOP_MESSAGE_LEAVE_TOOL, 0) != string::npos || messages.find(SHOP_MESSAGE_LEAVE_TOOL_ANGRY, 0) != string::npos) {
		/* we're most likely standing in a doorway, next to a shopkeeper.
		 * head for nearest CORRIDOR or FLOOR and drop pick-axe */
		int moves = 0;
		unsigned char dir = ILLEGAL_DIRECTION;
		dir = saiph->shortestPath(CORRIDOR, false, &moves);
		if (dir == ILLEGAL_DIRECTION)
			dir = saiph->shortestPath(FLOOR, false, &moves);

		if (dir == ILLEGAL_DIRECTION) {
			/* this is bad */
			Debug::warning() << "Unable to path to CORRIDOR or FLOOR from shopkeeper" << endl;
			return;
		}

		drop_pick_axe = true;
		command = dir;
		priority = PRIORITY_SHOP_DROP_DIGGING_TOOL;
	} else if (saiph->world->question && messages.find(MESSAGE_WHAT_TO_DROP, 0) == 0 && drop_pick_axe) {
		/* drop our tools */
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (i->second.name != "pick-axe" && i->second.name != "mattock")
				continue;
			command = i->first;
			priority = PRIORITY_CONTINUE_ACTION;
			drop_pick_axe = false;
			look_at_ground = true;
			return;
		}
	} else if (drop_pick_axe && saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] != OPEN_DOOR) {
		/* we should've moved away from shopkeeper now, drop the pick-axe */
		command = DROP;
		priority = PRIORITY_SHOP_DROP_DIGGING_TOOL;
	} else if (look_at_ground) {
		/* we'll look at ground after dropping the tools.
		 * this makes us aware of the stash,
		 * and the loot analyzer won't "visit" the stash after we move */
		/* FIXME:
		 * possibly not safe. what if another analyzer do something
		 * with higher priority?
		 * unlikely, but may be possible */
		command = LOOK;
		priority = PRIORITY_LOOK;
		look_at_ground = false;
	}
}

void Shop::analyze() {
	if (saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] == SHOP_TILE) {
		/* if we're standing on SHOP_TILE, check if we can see the shopkeeper.
		 * if we can't we probably killed him/her, and then we should remove the SHOP_TILE */
		bool shopkeeper_seen = false;
		for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
			if (!m->second.shopkeeper || !m->second.visible)
				continue;
			shopkeeper_seen = true;
			break;
		}
		if (!shopkeeper_seen) {
			/* can't see any shopkeeper, make the tile FLOOR */
			saiph->levels[saiph->position.level].setDungeonSymbol(saiph->position, FLOOR);
			/* we should also look at floor so Loot gets a chance to
			 * pick up the item as the last time Loot checked, this
			 * tile was a SHOP_TILE */
			command = LOOK;
			priority = PRIORITY_CONTINUE_ACTION;
			/* return because there's no need to check for a shopkeeper again */
			return;
		}
	}
	if (saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] != FLOOR && saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] != UNKNOWN_TILE)
		return; // not standing on FLOOR or UNKNOWN_TILE, no shop here (or detected already)

	for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
		if (!m->second.shopkeeper || !m->second.visible)
			continue;

		/* figure out if we're in the same room as the shopkeeper */
		int north = saiph->position.row;
		int south = saiph->position.row;
		int west = saiph->position.col;
		int east = saiph->position.col;

		unsigned char symbol = 0;

		symbol = saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col];
		while ((symbol == FLOOR || symbol == UNKNOWN_TILE) && north > MAP_ROW_BEGIN)
			symbol = saiph->levels[saiph->position.level].dungeonmap[--north][saiph->position.col];

		symbol = saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col];
		while ((symbol == FLOOR || symbol == UNKNOWN_TILE) && south < MAP_ROW_END)
			symbol = saiph->levels[saiph->position.level].dungeonmap[++south][saiph->position.col];

		symbol = saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col];
		while ((symbol == FLOOR || symbol == UNKNOWN_TILE) && west > MAP_COL_BEGIN)
			symbol = saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][--west];

		symbol = saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col];
		while ((symbol == FLOOR || symbol == UNKNOWN_TILE) && east < MAP_COL_END)
			symbol = saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][++east];

		if (m->first.row <= north || m->first.row >= south || m->first.col <= west || m->first.col >= east)
			return; // we're not in the same room as the shopkeeper

		Debug::notice() << "[Shop       ] bounds are (" << north << ", " << west << ", " << south << ", " << east << ")" << endl;

		/* mark all tiles within boundaries as SHOP_TILE */
		Point p;
		for (p.row = north + 1; p.row < south; ++p.row) {
			for (p.col = west + 1; p.col < east; ++p.col)
				saiph->levels[saiph->position.level].setDungeonSymbol(p, SHOP_TILE);
		}
		/* we should LOOK at floor to prevent Loot from picking
		 * up an item if we fall into a shop on an item we want
		 * before we get to mark the tiles as SHOP_TILE */
		command = LOOK;
		priority = PRIORITY_CONTINUE_ACTION;
	}
}
