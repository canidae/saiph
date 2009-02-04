#include <stdlib.h>
#include <stack>
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
	/* FIXME:
	 * this currently bugs. she's marking SHOP_TILE as FLOOR
	 * and picks up items. why? */
//	if (saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] == SHOP_TILE) {
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
	if (saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] != FLOOR && saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] != UNKNOWN_TILE)
		return; // not standing on FLOOR or UNKNOWN_TILE, no shop here (or detected already)

	for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
		if (!m->second.shopkeeper || !m->second.visible)
			continue;

		bool need_look = false;

		// We see a shopkeeper, so do a depth-first search to mark SHOP_TILEs
		memset(&visited, 0, (MAP_ROW_END + 1) * (MAP_COL_END + 1) * sizeof(bool));
		stack<Point> s;
		stack<Point> door;
		s.push(m->first);

		while (s.empty() == false) {
			Point top = s.top();
			s.pop();

			if (top.row < MAP_ROW_BEGIN || top.row > MAP_ROW_END ||
			    top.col < MAP_COL_BEGIN || top.col > MAP_COL_END ||
			    visited[top.row][top.col])
				continue;

			unsigned char symbol = saiph->levels[saiph->position.level].dungeonmap[top.row][top.col];
			switch (symbol) {
			case FLOOR:
			case UNKNOWN_TILE:
				Debug::notice() << "[Shop       ] Marking " << top << " as SHOP_TILE" << endl;
				saiph->levels[saiph->position.level].setDungeonSymbol(top, SHOP_TILE);

				// If we're marking our current location as a shop, look at ground
				if (top == (Point)saiph->position)
					need_look = true;

				// fall through to mark visited and allow neighbors to be examined
			case SHOP_TILE:
				visited[top.row][top.col] = true;

				s.push(Point(top.row + 1, top.col));
				s.push(Point(top.row - 1, top.col));
				s.push(Point(top.row, top.col + 1));
				s.push(Point(top.row, top.col - 1));
				break;
			case OPEN_DOOR:
			case CLOSED_DOOR:
				door.push(Point(top.row + 1, top.col));
				door.push(Point(top.row - 1, top.col));
				door.push(Point(top.row, top.col + 1));
				door.push(Point(top.row, top.col - 1));
				break;
			default:
				break;
			}
		}

#if 0 /* When we want to have a special tile, uncomment this */
		/* Mark the tile just inside as a place we can't sell.
		 * Find the SHOP_TILE next to the door and mark that.
		 */

		while (door.empty() == false) {
			Point top = door.top();
			door.pop();

			if (top.row < MAP_ROW_BEGIN || top.row > MAP_ROW_END ||
			    top.col < MAP_COL_BEGIN || top.col > MAP_COL_END)
				continue;

			if (saiph->levels[saiph->position.level].dungeonmap[top.row][top.col] == SHOP_TILE) {
				saiph->levels[saiph->position.level].dungeonmap[top.row][top.col] == SHOP_TILE_NO_SELL;
				break;
			}
		}
#endif

		/* we should LOOK at floor to prevent Loot from picking
		 * up an item if we fall into a shop on an item we want
		 * before we get to mark the tiles as SHOP_TILE */
		if (need_look) {
			command = LOOK;
			priority = PRIORITY_CONTINUE_ACTION;
		}
	}
}
