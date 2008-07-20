#include "Loot.h"

/* constructors */
Loot::Loot(Saiph *saiph) : Analyzer("Loot"), saiph(saiph) {
	memset(check_item, false, sizeof (check_item));
	check_item[WEAPON] = true;
	check_item[ARMOR] = true;
	check_item[RING] = true;
	check_item[AMULET] = true;
	check_item[TOOL] = true;
	check_item[FOOD] = true;
	check_item[POTION] = true;
	check_item[SCROLL] = true;
	check_item[SPELLBOOK] = true;
	check_item[WAND] = true;
	check_item[GOLD] = true;
	check_item[GEM] = true;
	check_item[STATUE] = true;
	// skipping boulder as that's a special item
	check_item[IRON_BALL] = true;
	check_item[CHAINS] = true;
	check_item[VENOM] = true;

	last_turn_inventory_check = INT_MIN;
}

/* methods */
void Loot::command(string *command) {
	*command = action;
	if (action == "i") {
		/* checking inventory, set last_turn_inventory_check */
		last_turn_inventory_check = saiph->world->player.turn;
	}
}

void Loot::finish() {
	/* figure out which stash to visit, if any */
	if (action == ":")
		return;
	if (saiph->world->player.turn > last_turn_inventory_check + LOOT_CHECK_INVENTORY_INTERVAL) {
		action = "i";
		priority = PRIORITY_LOOK;
		return;
	}
	int best_distance = INT_MAX;
	int best_priority = -1;
	action = "";
	/* pickup items */
	for (list<Request>::iterator p = pickup.begin(); p != pickup.end(); ++p) {
		if (p->coordinate.branch != saiph->current_branch || p->coordinate.level != saiph->current_level)
			continue;
		if (p->priority < best_priority)
			continue;
		int distance = -1;
		bool straight_line = false;
		unsigned char move = saiph->shortestPath(p->coordinate, false, &distance, &straight_line);
		if (move != ILLEGAL_MOVE && distance < best_distance) {
			best_distance = distance;
			best_priority = p->priority;
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

void Loot::inspect(const Point &point) {
	unsigned char s = saiph->world->view[point.row][point.col];
	int b = saiph->current_branch;
	int l = saiph->current_level;
	if (check_item[s]) {
		/* we're interested in this symbol */
		if (stashes[b][l][point.row][point.col].items.size() > 0) {
			/* we know of a stash here already */
			if (stashes[b][l][point.row][point.col].top_item == s) {
				/* top item is the same as last time we checked. */
				return;
			} else {
				/* top item changed, we should check this stash again */
			}
		} else {
			/* we've not seen a stash here before, we should check */
		}
		/* visit stash (again) */
		stashes[b][l][point.row][point.col].top_item = s;
		visit.push_back(point);
	}
}

bool Loot::request(const Request &request) {
	if (request.request == REQUEST_LOOT_STASH) {
		/* someone wants loot */
		loot.push_back(request.coordinate);
		return true;
	}
	return false;
}
