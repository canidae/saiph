#include "Loot.h"

/* constructors */
Loot::Loot(Saiph *saiph) : Analyzer("Loot"), saiph(saiph), dirty_inventory(true), dirty_stash(false), showing_inventory(false), showing_pickup(false), showing_drop(false) {
}

/* methods */
void Loot::analyze() {
	/* check inventory/stash if it's dirty */
	if (priority >= PRIORITY_LOOK)
		return;
	if (dirty_inventory) {
		checkInventory();
		return;
	}
	if (dirty_stash) {
		checkStash();
		return;
	}
	/* this is a hack for the rogue level where stairs look like '%' */
	if (saiph->levels[saiph->position.level].branch == BRANCH_ROGUE) {
		map<Point, Stash>::iterator s = saiph->levels[saiph->position.level].stashes.find(saiph->position);
		if (s != saiph->levels[saiph->position.level].stashes.end() && s->second.top_symbol == '%' && saiph->levels[saiph->position.level].dungeonmap[s->first.row][s->first.col] != STAIRS_UP && saiph->levels[saiph->position.level].dungeonmap[s->first.row][s->first.col] != STAIRS_DOWN) {
			command = LOOK;
			priority = PRIORITY_LOOK;
			return;
		}
	}

	/* loot stash we're standing on */
	if (priority >= LOOT_LOOT_STASH_PRIORITY)
		return;
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
				if (wantedItem(*i) == 0)
					continue;
				command = PICKUP;
				priority = LOOT_LOOT_STASH_PRIORITY;
				return;
			}
		}
	}

	/* drop unwanted stuff on STAIRS_UP and get safe Elbereth there */
	if (priority >= LOOT_DROP_ITEMS_PRIORITY)
		return;
	if (saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] == STAIRS_UP) {
		/* standing on stairs, drop unwanted stuff if any */
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (unwantedItem(i->second) == 0)
				continue;
			/* we got unwanted stuff */
			command = DROP;
			priority = LOOT_DROP_ITEMS_PRIORITY;
			return;
		}
		/* TODO: get down elbereth if there's a stash here */
	} else if (saiph->world->player.encumbrance > UNENCUMBERED) {
		/* we're not standing on stairs, but we're burdened or worse.
		 * go to stairs if we got unwanted stuff */
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (unwantedItem(i->second) == 0)
				continue;
			/* we got unwanted stuff */
			map<Point, int>::iterator up = saiph->levels[saiph->position.level].symbols[STAIRS_UP].begin();
			if (up != saiph->levels[saiph->position.level].symbols[STAIRS_UP].end()) {
				int moves = 0;
				unsigned char move = saiph->shortestPath(up->first, false, &moves);
				if (move != ILLEGAL_MOVE) {
					command = move;
					priority = LOOT_DROP_ITEMS_PRIORITY;
					return;
				}
			}
			break;
		}
	}

	if (priority >= LOOT_VISIT_STASH_PRIORITY || saiph->world->player.hallucinating)
		return;
	/* visit new/changed stashes unless hallucinating */
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
	if (dirty_inventory && command == INVENTORY) {
		/* we're showing our inventory */
		showing_inventory = true;
	} else if (dirty_stash && command == LOOK) {
		/* looked at ground, stash is no longer dirty */
		dirty_stash = false;
	}
}

void Loot::parseMessages(const string &messages) {
	if (saiph->got_pickup_menu) {
		/* looting */
		for (map<unsigned char, Item>::iterator p = saiph->pickup.begin(); p != saiph->pickup.end(); ++p) {
			int wanted = wantedItem(p->second);
			if (wanted == 0) {
				/* pick up none */
				continue;
			} else if (wanted >= p->second.count) {
				/* pick up all */
				command = p->first;
			} else {
				/* pick up some */
				stringstream tmp;
				tmp << wanted << p->first;
				command = tmp.str();
			}
			priority = PRIORITY_SELECT_ITEM;
			return;
		}
		/* if we're here, we should get next page or close list */
		showing_pickup = true;
		command = CLOSE_PAGE;
		priority = PRIORITY_CLOSE_ITEM_PAGE;
	} else if (saiph->got_drop_menu && saiph->levels[saiph->position.level].dungeonmap[saiph->position.row][saiph->position.col] == STAIRS_UP) {
		/* drop unwanted stuff */
		for (map<unsigned char, Item>::iterator d = saiph->drop.begin(); d != saiph->drop.end(); ++d) {
			if (d->second.name == "gold piece")
				continue; // don't drop gold
			int unwanted = unwantedItem(d->second);
			/* we'll "cheat" a bit here:
			 * we "forget" what we've marked to be dropped,
			 * so we'll reduce the item count in our inventory when we select it */
			if (unwanted == 0) {
				/* drop none */
				continue;
			} else if (unwanted >= d->second.count) {
				/* drop all */
				saiph->inventory[d->first].count = 0;
				command = d->first;
			} else {
				/* drop some */
				saiph->inventory[d->first].count -= unwanted;
				stringstream tmp;
				tmp << unwanted << d->first;
				command = tmp.str();
			}
			priority = PRIORITY_SELECT_ITEM;
			return;
		}
		/* if we're here, we should get next page or close list */
		showing_drop = true;
		command = CLOSE_PAGE;
		priority = PRIORITY_CLOSE_ITEM_PAGE;
	} else if (saiph->world->menu && showing_inventory) {
		/* we should close the page of the inventory we're showing */
		command = CLOSE_PAGE;
		priority = PRIORITY_CLOSE_ITEM_PAGE;
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
			/* we just had a pickup menu. check stash */
			checkStash();
			showing_pickup = false;
		} else if (showing_drop) {
			/* we just had a drop menu. check stash and inventory */
			checkInventory();
			checkStash();
			showing_drop = false;
		}
	}
	if (messages.find(LOOT_SEVERAL_OBJECTS_HERE, 0) != string::npos || messages.find(LOOT_MANY_OBJECTS_HERE, 0) != string::npos || messages.find(LOOT_SEVERAL_MORE_OBJECTS_HERE, 0) != string::npos || messages.find(LOOT_MANY_MORE_OBJECTS_HERE, 0) != string::npos) {
		/* several/many objects here. check stash */
		checkStash();
	}
	if (messages.find(LOOT_STOLE, 0) != string::npos) {
		/* some monster stole something, we should check our inventory */
		checkInventory();
	}
}

bool Loot::request(const Request &request) {
	if (request.request == REQUEST_DIRTY_INVENTORY) {
		/* someone wants to mark our inventory as dirty */
		checkInventory();
		return true;
	} else if (request.request == REQUEST_DIRTY_STASH) {
		/* someone wants to mark stash at our position dirty */
		checkStash();
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
void Loot::checkInventory() {
	dirty_inventory = true;
	if (priority >= PRIORITY_CHECK_INVENTORY)
		return;
	command = INVENTORY;
	priority = PRIORITY_CHECK_INVENTORY;
}

void Loot::checkStash() {
	dirty_stash = true;
	if (priority >= PRIORITY_LOOK)
		return;
	command = LOOK;
	priority = PRIORITY_LOOK;
}

int Loot::unwantedItem(const Item &item) {
	/* return how many we we should drop of given item */
	map<string, ItemWanted>::iterator i = items.find(item.name);
	if (i == items.end())
		return item.count; // item is not in our list
	if ((item.beatitude & i->second.beatitude) == 0)
		return item.count; // item does not have a beatitude we'll accept
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
		if (!item_in_group)
			continue;
		if (count <= g->second.amount)
			return 0;
		else
			return count - g->second.amount;
	}
	/* solitary items */
	if (i->second.amount <= 0)
		return item.count; // we don't desire this item
	/* figure out how many we got of this item already */
	int count = 0;
	for (map<unsigned char, Item>::iterator in = saiph->inventory.begin(); in != saiph->inventory.end(); ++in) {
		if (in->second.name != item.name)
			continue;
		count += in->second.count;
	}
	if (count <= i->second.amount)
		return 0;
	else
		return count - i->second.amount;
}

int Loot::wantedItem(const Item &item) {
	/* return how many we we should pick up of given item */
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
		if (!item_in_group)
			continue;
		if (count >= g->second.amount)
			return 0;
		else
			return g->second.amount - count;
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
		return 0;
	else
		return i->second.amount - count;
}
