#include "Loot.h"

/* constructors */
Loot::Loot(Saiph *saiph) : Analyzer("Loot"), saiph(saiph), dirty_inventory(true) {
}

/* methods */
void Loot::command(string *command) {
	*command = action;
	if (action == "i") {
		/* checking inventory, set dirty_inventory to false */
		dirty_inventory = false;
		action = " ";
	} else if (action == " ") {
		/* closing inventory/pickup list */
		action = "";
	} else if (action == "," && loot.find(saiph->position) != loot.end()) {
		/* we're looting a place, remove it from list */
		loot.erase(saiph->position);
	}
}

void Loot::finish() {
	/* check inventory, loot or visit stash */
	if (priority >= PRIORITY_LOOK)
		return; // we're probably listing inventory
	/* first check if some stashes have changed since last time */
	for (map<Point, Stash>::iterator s = saiph->stashes[saiph->position.branch][saiph->position.level].begin(); s != saiph->stashes[saiph->position.branch][saiph->position.level].end(); ++s) {
		map<Point, int>::iterator t = turn_last_changed[saiph->position.branch][saiph->position.level].find(s->first);
		if (t != turn_last_changed[saiph->position.branch][saiph->position.level].end() && s->second.turn_changed - t->second <= LOOT_REVISIT_STASH_TIME)
			continue; // not changed or changed too soon for us to care
		/* we should visit the stash */
		turn_last_changed[saiph->position.branch][saiph->position.level][s->first] = s->second.turn_changed;
		visit.push_back(s->first);
	}
	int best_distance = INT_MAX;
	int best_priority = ILLEGAL_PRIORITY;
	/* loot items */
	for (map<Coordinate, int>::iterator l = loot.begin(); l != loot.end(); ) {
		if (saiph->stashes[l->first.branch][l->first.level].find(l->first) == saiph->stashes[l->first.branch][l->first.level].end()) {
			/* this stash doesn't exist */
			loot.erase(l++);
			continue;
		}
		if (l->first.branch != saiph->position.branch || l->first.level != saiph->position.level) {
			++l;
			continue;
		}
		if (l->second < best_priority) {
			++l;
			continue;
		}
		int distance = -1;
		bool straight_line = false;
		unsigned char move = saiph->shortestPath(l->first, false, &distance, &straight_line);
		if (move != ILLEGAL_MOVE && distance < best_distance) {
			best_distance = distance;
			best_priority = l->second;
			action = move;
		}
		++l;
	}
	if (best_priority > priority && best_priority > ILLEGAL_PRIORITY) {
		/* we should pick up something */
		if (action[0] == REST) {
			/* infact, we should pick up something _here_ */
			action = PICKUP;
		}
		priority = best_priority;
	}
	if (LOOT_VISIT_STASH_PRIORITY < priority)
		return;
	/* visit stashes */
	best_distance = INT_MAX;
	unsigned char best_action = ILLEGAL_ACTION;
	for (list<Point>::iterator v = visit.begin(); v != visit.end(); ) {
		if (v->row == saiph->world->player.row && v->col == saiph->world->player.col) {
			/* we're on the stash, remove it from visit list */
			v = visit.erase(v);
			continue;
		}
		if (saiph->stashes[saiph->position.branch][saiph->position.level].find(*v) == saiph->stashes[saiph->position.branch][saiph->position.level].end()) {
			/* hmm, stash is gone */
			v = visit.erase(v);
			continue;
		}
		int distance = -1;
		bool straight_line = false;
		unsigned char move = saiph->shortestPath(*v, false, &distance, &straight_line);
		if (move != ILLEGAL_MOVE && distance < best_distance) {
			best_distance = distance;
			best_action = move;
		}
		++v;
	}
	if (best_distance < INT_MAX && LOOT_VISIT_STASH_PRIORITY > best_priority) {
		priority = LOOT_VISIT_STASH_PRIORITY;
		action = best_action;
	}
}

void Loot::parseMessages(const string &messages) {
	if (messages.find(MESSAGE_SEVERAL_OBJECTS_HERE, 0) != string::npos || messages.find(MESSAGE_MANY_OBJECTS_HERE, 0) != string::npos) {
		/* several/many objects here. we should look */
		action = ":";
		priority = PRIORITY_LOOK;
	} else if (saiph->world->menu && messages.find(MESSAGE_PICK_UP_WHAT, 0) != string::npos) {
		/* we're picking up stuff.
		 * analyzers will by themselves decide what to pick up, so we just try to close it */
		action = " ";
		priority = PRIORITY_PICKUP_STASH;
		/* and remember to remove this stash from "loot" */
		if (loot.find(saiph->position) != loot.end())
			loot.erase(saiph->position);
	} else if (saiph->world->menu && action == " ") {
		/* probably listing the inventory, close the menu */
		priority = PRIORITY_CONTINUE_ACTION;
	}
}

bool Loot::request(const Request &request) {
	if (request.request == REQUEST_LOOT_STASH) {
		/* someone wants loot */
		map<Coordinate, int>::iterator l = loot.find(request.coordinate);
		if (l != loot.end()) {
			/* this stash has already been requested by someone else.
			 * check if priority is higher */
			if (l->second < request.priority)
				l->second = request.priority;
		} else {
			/* this stash has not been requested looted before */
			loot[request.coordinate] = request.priority;
		}
		return true;
	} else if (request.request == REQUEST_DIRTY_INVENTORY) {
		/* someone wants to mark our inventory as dirty */
		dirty_inventory = true;
		return true;
	} else if (request.request == REQUEST_UPDATED_INVENTORY) {
		/* someone needs an updated inventory */
		if (dirty_inventory) {
			/* it's dirty, we must look */
			action = "i";
			priority = request.priority;
		}
		return true;
	}
	return false;
}
