#include "Loot.h"

/* constructors */
Loot::Loot(Saiph *saiph) : Analyzer("Loot"), saiph(saiph) {
	last_turn_inventory_check = INT_MIN;
}

/* methods */
void Loot::command(string *command) {
	*command = action;
	if (action == "i") {
		/* checking inventory, set last_turn_inventory_check */
		last_turn_inventory_check = saiph->world->player.turn;
		action = "";
	}
}

void Loot::finish() {
	/* check inventory, loot or visit stash */
	if (saiph->world->player.turn > last_turn_inventory_check + LOOT_CHECK_INVENTORY_INTERVAL) {
		action = "i";
		priority = PRIORITY_LOOK;
		return;
	}
	int best_distance = INT_MAX;
	int best_priority = -1;
	action = "";
	/* loot items */
	for (list<LootStash>::iterator l = loot.begin(); l != loot.end(); ++l) {
		if (l->branch != saiph->position.branch || l->level != saiph->position.level)
			continue;
		if (l->priority < best_priority)
			continue;
		int distance = -1;
		bool straight_line = false;
		unsigned char move = saiph->shortestPath(*l, false, &distance, &straight_line);
		if (move != ILLEGAL_MOVE && distance < best_distance) {
			best_distance = distance;
			best_priority = l->priority;
			action = move;
		}
	}
	if (best_priority >= LOOT_VISIT_STASH_PRIORITY && action.size() > 0) {
		/* we should pick up something */
		if (action[0] == REST) {
			/* infact, we should pick up something _here_ */
			action = PICKUP;
		}
		priority = best_priority;
		return;
	}
	best_distance = INT_MAX;
	action = "";
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
			action = move;
		}
		++v;
	}
	if (best_distance < INT_MAX)
		priority = LOOT_VISIT_STASH_PRIORITY;
}

bool Loot::request(const Request &request) {
	if (request.request == REQUEST_LOOT_STASH) {
		/* someone wants loot */
		loot.push_back(LootStash(request.coordinate, request.priority));
		return true;
	}
	return false;
}
