#include "Loot.h"

/* constructors */
Loot::Loot(Saiph *saiph) : Analyzer("Loot"), saiph(saiph), update_inventory(true) {
}

/* methods */
void Loot::command(string *command) {
	*command = action;
	if (action == "i") {
		/* checking inventory, set update_inventory to false */
		update_inventory = false;
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
	/* first check if some stashes have changed since last time */
	for (vector<Point>::iterator s = saiph->itemtracker->changed.begin(); s != saiph->itemtracker->changed.end(); ++s)
		visit.push_back(*s);
	/* check inventory */
	if (PRIORITY_LOOK > priority && update_inventory) {
		action = "i";
		priority = PRIORITY_LOOK;
		return;
	}
	int best_distance = INT_MAX;
	int best_priority = ILLEGAL_PRIORITY;
	/* loot items */
	for (map<Coordinate, int>::iterator l = loot.begin(); l != loot.end(); ++l) {
		if (l->first.branch != saiph->position.branch || l->first.level != saiph->position.level)
			continue;
		if (l->second < best_priority)
			continue;
		int distance = -1;
		bool straight_line = false;
		unsigned char move = saiph->shortestPath(l->first, false, &distance, &straight_line);
		if (move != ILLEGAL_MOVE && distance < best_distance) {
			best_distance = distance;
			best_priority = l->second;
			action = move;
		}
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
	} else if (request.request == REQUEST_LIST_INVENTORY) {
		/* someone needs to check our inventory */
		update_inventory = true;
		return true;
	}
	return false;
}
