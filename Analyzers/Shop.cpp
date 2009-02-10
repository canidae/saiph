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
			Debug::warning(saiph->last_turn) << SHOP_DEBUG_NAME << "Unable to path to CORRIDOR or FLOOR from shopkeeper" << endl;
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
		/* request dirty inventory */
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
	} else if (drop_pick_axe && saiph->getDungeonSymbol() != OPEN_DOOR) {
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
	unsigned char symbol = saiph->getDungeonSymbol();
	/* FIXME:
	 * this currently bugs. she's marking SHOP_TILE as FLOOR
	 * and picks up items. why? */
//	if (symbol == SHOP_TILE) {
//		/* if we're standing on SHOP_TILE, check if we can see the shopkeeper.
//		 * if we can't we probably killed him/her, and then we should remove the SHOP_TILE */
//		bool shopkeeper_seen = false;
//		for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
//			if (!m->second.shopkeeper || !m->second.visible)
//				continue;
//			shopkeeper_seen = true;
//			break;
//		}
//		if (!shopkeeper_seen) {
//			/* can't see any shopkeeper, make the tile FLOOR */
//			saiph->levels[saiph->position.level].setDungeonSymbol(saiph->position, FLOOR);
//			/* we should also look at floor so Loot gets a chance to
//			 * pick up the item as the last time Loot checked, this
//			 * tile was a SHOP_TILE */
//			command = LOOK;
//			priority = PRIORITY_CONTINUE_ACTION;
//			/* return because there's no need to check for a shopkeeper again */
//			return;
//		}
//	}
	if (symbol != FLOOR && symbol != UNKNOWN_TILE)
		return; // not standing on FLOOR or UNKNOWN_TILE, no shop here (or detected already)

	for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
		if (!m->second.shopkeeper || !m->second.visible)
			continue;

		/* figure out if we're in the same room as the shopkeeper */
		Point nw = saiph->position;
		Point se = saiph->position;

		/* find north corner */
		symbol = saiph->getDungeonSymbol();
		while (--nw.row && (symbol == FLOOR || symbol == UNKNOWN_TILE))
			symbol = saiph->getDungeonSymbol(nw);

		/* find west corner */
		symbol = saiph->getDungeonSymbol();
		while (--nw.col && (symbol == FLOOR || symbol == UNKNOWN_TILE))
			symbol = saiph->getDungeonSymbol(nw);

		/* find south corner */
		symbol = saiph->getDungeonSymbol();
		while (++se.row && (symbol == FLOOR || symbol == UNKNOWN_TILE))
			symbol = saiph->getDungeonSymbol(se);

		/* find east corner */
		symbol = saiph->getDungeonSymbol();
		while (++se.col && (symbol == FLOOR || symbol == UNKNOWN_TILE))
			symbol = saiph->getDungeonSymbol(se);

		if (m->first.row <= nw.row || m->first.col <= nw.col || m->first.row >= se.row || m->first.col >= se.col)
			return; // we're not in the same room as the shopkeeper

		Debug::notice(saiph->last_turn) << SHOP_DEBUG_NAME << "bounds are " << nw << " to " << se << endl;

		/* mark all tiles within boundaries as SHOP_TILE */
		Point p;
		for (p.row = nw.row + 1; p.row < se.row; ++p.row) {
			for (p.col = nw.col + 1; p.col < se.col; ++p.col)
				saiph->setDungeonSymbol(p, SHOP_TILE);
		}
		/* we should LOOK at floor to prevent Loot from picking
		 * up an item if we fall into a shop on an item we want
		 * before we get to mark the tiles as SHOP_TILE */
		command = LOOK;
		priority = PRIORITY_CONTINUE_ACTION;
	}
}
