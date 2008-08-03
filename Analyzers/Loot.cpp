#include "Loot.h"

/* constructors */
Loot::Loot(Saiph *saiph) : Analyzer("Loot"), saiph(saiph), dirty_inventory(true), listing_stuff(false) {
}

/* methods */
void Loot::analyze() {
	/* go to a stash requested visited */
	int min_moves = INT_MAX;
	unsigned char best_move = ILLEGAL_MOVE;
	for (map<Coordinate, int>::iterator v = visit_stash.begin(); v != visit_stash.end(); ) {
		if (v->second < priority) {
			/* got something else to do with a higher priority */
			++v;
			continue;
		}
		if (saiph->levels[v->first.level].stashes.find(v->first) == saiph->levels[v->first.level].stashes.end()) {
			/* hmm, there's no stash here */
			visit_stash.erase(v++);
			continue;
		}
		int moves = 0;
		unsigned char move = saiph->shortestPath(v->first, false, &moves);
		if (move != ILLEGAL_MOVE && moves < min_moves) {
			best_move = move;
			min_moves = moves;
			if (best_move == MOVE_NOWHERE) {
				/* standing on stash */
				visit_stash.erase(v);
			} else {
				/* moving towards stash */
				command = best_move;
				priority = v->second;
			}
		}
		++v;
	}

	/* discover stashes (meaning, non-requested visit of new/changed stashes) */
	if (priority >= LOOT_DISCOVER_STASH_PRIORITY)
		return;
	min_moves = INT_MAX;
	best_move = ILLEGAL_MOVE;
	for (map<Point, Stash>::iterator s = saiph->levels[saiph->position.level].stashes.begin(); s != saiph->levels[saiph->position.level].stashes.end(); ++s) {
		map<Coordinate, int>::iterator d = discover_stash.find(Coordinate(saiph->position.level, s->first));
		if (d == discover_stash.end() || d->second != s->second.turn_changed) {
			/* new or changed stash, visit it if it's closer */
			int moves = 0;
			unsigned char move = saiph->shortestPath(s->first, false, &moves);
			if (move != ILLEGAL_MOVE && moves < min_moves) {
				best_move = move;
				min_moves = moves;
			}
		}
	}
	if (best_move != ILLEGAL_MOVE) {
		if (best_move == MOVE_NOWHERE) {
			/* standing on stash */
			map<Point, Stash>::iterator s = saiph->levels[saiph->position.level].stashes.find(saiph->position);
			if (s == saiph->levels[saiph->position.level].stashes.end())
				discover_stash.erase(saiph->position);
			else
				discover_stash[saiph->position] = s->second.turn_changed;
		} else {
			/* moving towards stash */
			command = best_move;
			priority = LOOT_DISCOVER_STASH_PRIORITY;
		}
	}
}

void Loot::parseMessages(const string &messages) {
	if (listing_stuff && !saiph->world->menu) {
		/* no longer listing something */
		listing_stuff = false;
	}
	if (messages.find(MESSAGE_SEVERAL_OBJECTS_HERE, 0) != string::npos || messages.find(MESSAGE_MANY_OBJECTS_HERE, 0) != string::npos) {
		/* several/many objects here. we should look */
		command = LOOK;
		priority = PRIORITY_LOOK;
	} else if (messages.find(MESSAGE_PICK_UP_WHAT, 0) != string::npos) {
		listing_stuff = true;
	} else if (saiph->world->menu && command == INVENTORY) {
		listing_stuff = true;
		dirty_inventory = false;
	}
	if (listing_stuff) {
		/* we're listing something (inventory, pickup, drop).
		 * we just try to close the list, the analyzers will pick up or drop at their leisure */
		command = NEXT_PAGE;
		priority = PRIORITY_CONTINUE_ACTION;
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
	} else if (request.request == REQUEST_UPDATED_INVENTORY) {
		/* someone needs an updated inventory */
		if (dirty_inventory && request.priority > priority) {
			/* it's dirty, we must look */
			command = INVENTORY;
			priority = request.priority;
		}
		return true;
	}
	return false;
}
