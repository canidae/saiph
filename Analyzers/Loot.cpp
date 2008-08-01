#include "Loot.h"

/* constructors */
Loot::Loot(Saiph *saiph) : Analyzer("Loot"), saiph(saiph), dirty_inventory(true) {
}

/* methods */
void Loot::complete() {
	if (command == "i") {
		/* checking inventory, set dirty_inventory to false */
		dirty_inventory = false;
		command = " ";
	} else if (command == "," && loot.find(saiph->position) != loot.end()) {
		/* we're looting a place, remove it from list */
		loot.erase(saiph->position);
	}
}

void Loot::finish() {
	/* close inventory/pickup list */
	if (!saiph->world->menu && command == " ")
		command = "";
	/* check inventory, loot or visit stash */
	if (priority >= PRIORITY_LOOK)
		return; // we're probably listing inventory
	/* first check if some stashes have changed since last time */
	for (map<Point, Stash>::iterator s = saiph->levels[saiph->position.level].stashes.begin(); s != saiph->levels[saiph->position.level].stashes.end(); ++s) {
		map<Point, int>::iterator t = turn_last_changed[saiph->position.level].find(s->first);
		if (t != turn_last_changed[saiph->position.level].end() && s->second.turn_changed - t->second <= LOOT_REVISIT_STASH_TIME)
			continue; // not changed or changed too soon for us to care
		/* we should visit the stash */
		turn_last_changed[saiph->position.level][s->first] = s->second.turn_changed;
		visit.push_back(s->first);
	}
	int best_distance = INT_MAX;
	int best_priority = ILLEGAL_PRIORITY;
	/* loot items */
	for (map<Coordinate, int>::iterator l = loot.begin(); l != loot.end(); ) {
		if (saiph->levels[l->first.level].stashes.find(l->first) == saiph->levels[l->first.level].stashes.end()) {
			/* this stash doesn't exist */
			loot.erase(l++);
			continue;
		}
		if (l->first.level != saiph->position.level) {
			++l;
			continue;
		}
		if (l->second < best_priority) {
			++l;
			continue;
		}
		int distance = -1;
		unsigned char move = saiph->shortestPath(l->first, false, &distance);
		if (move != ILLEGAL_MOVE && distance < best_distance) {
			best_distance = distance;
			best_priority = l->second;
			command = move;
		}
		++l;
	}
	if (best_priority > priority && best_priority > ILLEGAL_PRIORITY) {
		/* we should pick up something */
		if (command[0] == REST) {
			/* infact, we should pick up something _here_ */
			command = PICKUP;
		}
		priority = best_priority;
	}
	if (LOOT_VISIT_STASH_PRIORITY < priority)
		return;
	/* visit stashes */
	best_distance = INT_MAX;
	unsigned char best_command = ILLEGAL_ACTION;
	for (list<Point>::iterator v = visit.begin(); v != visit.end(); ) {
		if (v->row == saiph->world->player.row && v->col == saiph->world->player.col) {
			/* we're on the stash, remove it from visit list */
			v = visit.erase(v);
			continue;
		}
		if (saiph->levels[saiph->position.level].stashes.find(*v) == saiph->levels[saiph->position.level].stashes.end()) {
			/* hmm, stash is gone */
			v = visit.erase(v);
			continue;
		}
		int distance = -1;
		unsigned char move = saiph->shortestPath(*v, false, &distance);
		if (move != ILLEGAL_MOVE && distance < best_distance) {
			best_distance = distance;
			best_command = move;
		}
		++v;
	}
	if (best_distance < INT_MAX && LOOT_VISIT_STASH_PRIORITY > best_priority) {
		priority = LOOT_VISIT_STASH_PRIORITY;
		command = best_command;
	}
}

void Loot::parseMessages(const string &messages) {
	if (messages.find(MESSAGE_SEVERAL_OBJECTS_HERE, 0) != string::npos || messages.find(MESSAGE_MANY_OBJECTS_HERE, 0) != string::npos) {
		/* several/many objects here. we should look */
		command = ":";
		priority = PRIORITY_LOOK;
	} else if (saiph->world->menu && messages.find(MESSAGE_PICK_UP_WHAT, 0) != string::npos) {
		/* we're picking up stuff.
		 * analyzers will by themselves decide what to pick up, so we just try to close it */
		command = " ";
		priority = PRIORITY_CLOSE_ITEM_LIST;
		/* and remember to remove this stash from "loot" */
		if (loot.find(saiph->position) != loot.end())
			loot.erase(saiph->position);
	} else if (saiph->world->menu && command == " ") {
		/* probably listing the inventory, close the menu */
		priority = PRIORITY_CONTINUE_ACTION;
	}
}

bool Loot::request(const Request &request) {
	if (request.request == REQUEST_LOOT_STASH) {
		/* someone wants loot */
		/* if we see a white '@' then don't loot */
		for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
			if (m->second.symbol == '@' && m->second.color == WHITE && m->second.visible)
				return false;
		}
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
			command = "i";
			priority = request.priority;
		}
		return true;
	}
	return false;
}
