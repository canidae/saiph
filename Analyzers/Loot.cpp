#include "Loot.h"

/* constructors */
Loot::Loot(Saiph *saiph) : Analyzer("Loot"), saiph(saiph), dirty_inventory(true), dirty_stash(false), showing_inventory(false), showing_pickup(false) {
}

/* methods */
void Loot::analyze() {
	/* check inventory/stash if it's dirty */
	if (dirty_inventory) {
		command = INVENTORY;
		priority = PRIORITY_LOOK;
		return;
	} else if (dirty_stash) {
		command = LOOK;
		priority = PRIORITY_LOOK;
		return;
	}
	if (priority >= LOOT_LOOT_STASH_PRIORITY)
		return;
	/* loot stash we're standing on */
	if (saiph->on_ground != NULL) {
		/* if we see a white '@' then don't loot */
		bool doloot = true;
		for (map<Point, Monster>::iterator m = saiph->levels[saiph->position.level].monsters.begin(); m != saiph->levels[saiph->position.level].monsters.end(); ++m) {
			if (m->second.symbol != '@' || m->second.color != WHITE || !m->second.visible)
				continue;
			doloot = false;
			break;
		}
		if (doloot) {
			for (list<Item>::iterator i = saiph->on_ground->items.begin(); i != saiph->on_ground->items.end(); ++i) {
				int wanted = pickupItem(*i);
				if (wanted == 0)
					continue;
				command = PICKUP;
				priority = LOOT_LOOT_STASH_PRIORITY;
				return;
			}
		}
	}

	/* visit new/changed stashes */
	if (priority >= LOOT_VISIT_STASH_PRIORITY)
		return;
	int min_moves = INT_MAX;
	for (map<Point, Stash>::iterator s = saiph->levels[saiph->position.level].stashes.begin(); s != saiph->levels[saiph->position.level].stashes.end(); ++s) {
		map<Coordinate, int>::iterator v = visit_stash.find(Coordinate(saiph->position.level, s->first));
		if (v != visit_stash.end() && v->second == s->second.turn_changed)
			continue; // stash is unchanged
		/* new or changed stash, visit it if it's closer */
		int moves = 0;
		unsigned char move = saiph->shortestPath(s->first, false, &moves);
		if (move == MOVE_NOWHERE) {
			/* standing on stash, update turn_changed */
			visit_stash[saiph->position] = s->second.turn_changed;
		} else if (move != ILLEGAL_MOVE && moves < min_moves) {
			/* move towards stash */
			min_moves = moves;
			command = move;
			priority = LOOT_VISIT_STASH_PRIORITY;
		}
	}
}

void Loot::complete() {
	if (command == INVENTORY) {
		/* we're showing our inventory */
		showing_inventory = true;
	}
}

void Loot::parseMessages(const string &messages) {
	if (saiph->pickup.size() > 0) {
		/* looting */
		for (map<unsigned char, Item>::iterator p = saiph->pickup.begin(); p != saiph->pickup.end(); ++p) {
			int wanted = pickupItem(p->second);
			if (wanted == 0)
				continue;
			if (wanted <= p->second.count) {
				/* pick them all up */
				command = p->first;
			} else {
				/* only pick some up */
				stringstream tmp;
				tmp << wanted << p->first;
				command = tmp.str();
			}
			priority = PRIORITY_SELECT_ITEM;
			return;
		}
		/* if we're here, we should get next page or close list */
		showing_pickup = true;
		command = NEXT_PAGE;
		priority = PRIORITY_CLOSE_ITEM_LIST;
	} else if (saiph->world->menu && showing_inventory) {
		/* we should close the page of the inventory we're showing */
		command = NEXT_PAGE;
		priority = PRIORITY_CLOSE_ITEM_LIST;
		return;
	} else if (!saiph->world->menu) {
		if (showing_inventory) {
			/* we showed our inventory, but now it's closed */
			dirty_inventory = false;
			showing_inventory = false;
			/* also announce that it's no longer dirty */
			req.request = REQUEST_UPDATED_INVENTORY;
			saiph->request(req);
		} else if (showing_pickup) {
			/* we just had a pickup menu. we should look on ground what's left.
			 * why not just remove items as we pick them up?
			 * because it takes a turn, and monsters _love_ throwing stuff at her that turn,
			 * which makes her then ignore whatever they threw at her.
			 * it's a zero turn thingy, only annoying for those who watch */
			command = LOOK;
			priority = PRIORITY_LOOK;
			showing_pickup = false;
		}
	}
	if (messages.find(LOOT_SEVERAL_OBJECTS_HERE, 0) != string::npos || messages.find(LOOT_MANY_OBJECTS_HERE, 0) != string::npos) {
		/* several/many objects here. we should look */
		command = LOOK;
		priority = PRIORITY_LOOK;
	}
	if (messages.find(LOOT_STOLE, 0) != string::npos) {
		/* some monster stole something, we should check our inventory */
		dirty_inventory = true;
		command = INVENTORY;
		priority = PRIORITY_LOOK;
	}
}

bool Loot::request(const Request &request) {
	if (request.request == REQUEST_DIRTY_INVENTORY) {
		/* someone wants to mark our inventory as dirty */
		dirty_inventory = true;
		command = INVENTORY;
		priority = PRIORITY_LOOK;
		return true;
	} else if (request.request == REQUEST_DIRTY_STASH) {
		/* someone wants to mark stash at our position dirty */
		dirty_stash = true;
		command = LOOK;
		priority = PRIORITY_LOOK;
		return true;
	} else if (request.request == REQUEST_ITEM_GROUP_SET_AMOUNT) {
		/* set total amount of items in the given group */
		int amount = atoi(request.data.c_str());
		if (amount <= 0)
			return false; // group is useless with 0 or fewer items
		groups[request.value].amount = amount;
		return true;
	} else if (request.request == REQUEST_ITEM_GROUP_ADD) {
		/* add item to group */
		if (items.find(request.data) == items.end()) {
			/* we need to add an entry about this item */
			items[request.data].amount = 0;
			items[request.data].beatitude = request.status;
		}
		groups[request.value].items.push_back(request.data);
		return true;
	} else if (request.request == REQUEST_ITEM_PICKUP) {
		/* pick up items that are not part of a group */
		items[request.data].amount = request.value;
		items[request.data].beatitude = request.status;
		return true;
	}
	return false;
}

/* private methods */
int Loot::pickupItem(const Item &item) {
	/* return how many we want of this item */
	map<string, ItemWanted>::iterator i = items.find(item.name);
	if (i == items.end())
		return 0; // item is not in our list
	if ((item.beatitude & i->second.beatitude) == 0)
		return 0; // item does not have a beatitude we'll accept
	/* groups */
	for (map<int, ItemGroup>::iterator g = groups.begin(); g != groups.end(); ++g) {
		/* figure out how many items we already got in this group */
		int count = 0;
		int item_in_group = false;
		for (vector<string>::iterator gi = g->second.items.begin(); gi != g->second.items.end(); ++gi) {
			for (map<unsigned char, Item>::iterator in = saiph->inventory.begin(); in != saiph->inventory.end(); ++in) {
				if (in->second.name != *gi)
					continue;
				count += in->second.count;
			}
			if (*gi == item.name) {
				/* item is in group.
				 * we'll also break here so she'll pick up better items in group,
				 * even if that means she'll exceed the limit.
				 * otherwise she won't pick up eg. elven daggers when she's full on orcish daggers */
				item_in_group = true;
				break;
			}
		}
		if (!item_in_group || count >= g->second.amount)
			continue;
		if (count + item.count <= g->second.amount) {
			/* we want all of them */
			return item.count;
		} else {
			/* we only want some of them */
			return g->second.amount - count - item.count;
		}
	}
	/* solitary items */
	if (i->second.amount <= 0)
		return 0; // we don't desire this item
	/* figure out how many we got of this item already */
	int count = 0;
	for (map<unsigned char, Item>::iterator in = saiph->inventory.begin(); in != saiph->inventory.end(); ++in) {
		if (in->second.name != item.name)
			continue;
		count += in->second.count;
	}
	if (count >= i->second.amount)
		return 0; // got enough of this weapon already
	if (count + item.count <= i->second.amount) {
		/* we want all of them */
		return item.count;
	} else {
		/* we only want some of them */
		return i->second.amount - count - item.count;
	}
}
