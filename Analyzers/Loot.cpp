#include "Loot.h"

/* constructors */
Loot::Loot(Saiph *saiph) : Analyzer("Loot"), saiph(saiph), dirty_inventory(true), listing_inventory(false), listing_items(false) {
}

/* methods */
void Loot::analyze() {
	/* list inventory if it's dirty */
	if (dirty_inventory) {
		command = INVENTORY;
		priority = PRIORITY_LOOK;
		return;
	}
	/* go to a stash requested visited */
	int min_moves = INT_MAX;
	for (map<Coordinate, int>::iterator v = visit_stash.begin(); v != visit_stash.end(); ) {
		if (v->second < priority) {
			/* got something else to do with a higher priority */
			++v;
			continue;
		} else if (saiph->levels[v->first.level].stashes.find(v->first) == saiph->levels[v->first.level].stashes.end()) {
			/* hmm, there's no stash here */
			visit_stash.erase(v++);
			continue;
		} else if (saiph->levels[v->first.level].stashes.find(saiph->position) != saiph->levels[v->first.level].stashes.end()) {
			/* standing on stash, remove it from visit list */
			visit_stash.erase(v++);
			continue;
		}
		int moves = 0;
		unsigned char move = saiph->shortestPath(v->first, false, &moves);
		if (move != ILLEGAL_MOVE && moves < min_moves) {
			min_moves = moves;
			command = move;
			priority = v->second;
		}
		++v;
	}

	/* discover stashes (meaning, non-requested visit of new/changed stashes) */
	if (priority >= LOOT_DISCOVER_STASH_PRIORITY)
		return;
	min_moves = INT_MAX;
	for (map<Point, Stash>::iterator s = saiph->levels[saiph->position.level].stashes.begin(); s != saiph->levels[saiph->position.level].stashes.end(); ++s) {
		map<Coordinate, int>::iterator d = discover_stash.find(Coordinate(saiph->position.level, s->first));
		if (d == discover_stash.end() || d->second != s->second.turn_changed) {
			/* new or changed stash, visit it if it's closer */
			int moves = 0;
			unsigned char move = saiph->shortestPath(s->first, false, &moves);
			if (move == MOVE_NOWHERE) {
				/* standing on stash, update turn_changed */
				discover_stash[saiph->position] = s->second.turn_changed;
			} else if (move != ILLEGAL_MOVE && moves < min_moves) {
				/* move towards stash */
				min_moves = moves;
				command = move;
				priority = LOOT_DISCOVER_STASH_PRIORITY;
			}
		}
	}
}

void Loot::parseMessages(const string &messages) {
	if (!saiph->world->menu) {
		if (listing_inventory) {
			/* just listed inventory, it's no longer dirty */
			dirty_inventory = false;
			listing_inventory = false;
		}
		if (listing_items) {
			/* we just modified this stash,
			 * look at what's on the floor unless we think the stash is gone */
			listing_items = false;
			if (saiph->levels[saiph->position.level].stashes.find(saiph->position) != saiph->levels[saiph->position.level].stashes.end()) {
				command = LOOK;
				priority = PRIORITY_LOOK;
				return;
			}
		}
	}
	if (messages.find(LOOT_SEVERAL_OBJECTS_HERE, 0) != string::npos || messages.find(LOOT_MANY_OBJECTS_HERE, 0) != string::npos) {
		/* several/many objects here. we should look */
		command = LOOK;
		priority = PRIORITY_LOOK;
	} else if (messages.find(MESSAGE_PICK_UP_WHAT, 0) != string::npos) {
		listing_items = true;
	} else if (saiph->world->menu && command == INVENTORY) {
		listing_inventory = true;
	} else if (messages.find(MESSAGE_NOT_CARRYING_ANYTHING, 0) != string::npos || messages.find(MESSAGE_NOT_CARRYING_ANYTHING_EXCEPT_GOLD, 0) != string::npos) {
		/* we won't get a list when we're not carrying anything (except gold) */
		dirty_inventory = false;
	} else if (messages.find(LOOT_STOLE, 0) != string::npos) {
		dirty_inventory = true;
	}
	if (listing_inventory || listing_items) {
		/* we're listing something (inventory, pickup, drop).
		 * we just try to close the list, the analyzers will pick up or drop at their leisure */
		command = NEXT_PAGE;
		priority = PRIORITY_CLOSE_ITEM_LIST;
	}
}

bool Loot::request(const Request &request) {
	if (request.request == REQUEST_VISIT_STASH) {
		/* someone wants us to visit a stash */
		map<Coordinate, int>::iterator v = visit_stash.find(request.coordinate);
		if (v != visit_stash.end()) {
			/* already got stash in list, does this request have a higher priority? */
			v->second = max(request.priority, v->second);
		} else if (request.coordinate.level >= 0 && request.coordinate.level < (int) saiph->levels.size()) {
			/* add location */
			visit_stash[request.coordinate] = request.priority;
		} else {
			/* can't visit this stash for some reason */
			return false;
		}
		return true;
	} else if (request.request == REQUEST_LOOT_STASH) {
		/* someone wants something on the ground here */
		/* if we see a white '@' then don't loot */
		for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
			if (m->second.symbol == '@' && m->second.color == WHITE && m->second.visible)
				return false;
		}
		if (request.priority <= priority)
			return false;
		command = PICKUP;
		priority = request.priority;
		return true;
	} else if (request.request == REQUEST_DIRTY_INVENTORY) {
		/* someone wants to mark our inventory as dirty */
		dirty_inventory = true;
		return true;
	}
	return false;
}
