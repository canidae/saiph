#include <stdlib.h>
#include "Loot.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Loot::Loot(Saiph *saiph) : Analyzer("Loot"), saiph(saiph), dirty_inventory(true), dirty_stash(false), showing_inventory(false), showing_pickup(false), showing_drop(false) {
}

/* methods */
void Loot::analyze() {
	if (saiph->on_ground != NULL) {
		/* set visit_stash when we stand on a stash */
		visit_stash[saiph->position] = saiph->on_ground->turn_changed;
		if (visit_old_stash == saiph->position)
			visitOldStash(); // check if there are other old stashes we wish to visit
	}
	/* check that we don't have anything more important to do */
	if (priority >= PRIORITY_LOOK)
		return;
	/* check inventory/stash if it's dirty */
	if (dirty_inventory) {
		checkInventory();
		return;
	}
	if (dirty_stash) {
		checkStash();
		return;
	}
	if (name_items.size() > 0 || call_items.size() > 0) {
		/* we got items to name or call */
		command = NAME;
		priority = PRIORITY_LOOK;
		return;
	}

	/* loot stash we're standing on unless full knapsack or burdened */
	if (priority >= PRIORITY_LOOT_LOOT_STASH)
		return;
	unsigned char symbol = saiph->getDungeonSymbol();
	if (saiph->on_ground != NULL && saiph->inventory.size() < KNAPSACK_LIMIT && saiph->world->player.encumbrance < BURDENED) {
		/* only loot if we're not on a SHOP_TILE */
		if (symbol != SHOP_TILE) {
			for (list<Item>::iterator i = saiph->on_ground->items.begin(); i != saiph->on_ground->items.end(); ++i) {
				if (pickupItem(*i) == 0)
					continue;
				command = PICKUP;
				priority = PRIORITY_LOOT_LOOT_STASH;
				return;
			}
		}
	}

	/* drop unwanted stuff on STAIRS_UP and get safe Elbereth there */
	if (priority >= PRIORITY_LOOT_DROP_ITEMS)
		return;
	if (symbol == STAIRS_UP) {
		/* standing on stairs, drop unwanted stuff if any */
		for (map<unsigned char, Item>::iterator i = saiph->inventory.begin(); i != saiph->inventory.end(); ++i) {
			if (dropItem(i->second) == 0)
				continue;
			/* we got unwanted stuff */
			command = DROP_MENU;
			priority = PRIORITY_LOOT_DROP_ITEMS;
			return;
		}
		/* TODO: get down elbereth if there's a stash here */
	}

	if (priority >= PRIORITY_LOOT_VISIT_STASH || saiph->world->player.hallucinating || saiph->world->player.blind || saiph->world->player.encumbrance > UNENCUMBERED)
		return;
	/* visit new/changed stashes unless hallucinating, blind or too encumbered */
	int min_moves = INT_MAX;
	for (map<Point, Stash>::iterator s = saiph->levels[saiph->position.level].stashes.begin(); s != saiph->levels[saiph->position.level].stashes.end(); ++s) {
		map<Coordinate, int>::iterator v = visit_stash.find(Coordinate(saiph->position.level, s->first));
		if (v != visit_stash.end() && v->second == s->second.turn_changed)
			continue; // stash is unchanged
		/* unvisited stash, visit it if it's closer */
		int moves = 0;
		unsigned char dir = saiph->shortestPath(s->first, false, &moves);
		if (dir != NOWHERE && dir != ILLEGAL_DIRECTION && moves < min_moves) {
			/* move towards stash */
			min_moves = moves;
			command = dir;
			priority = PRIORITY_LOOT_VISIT_STASH;
		}
	}

	if (priority >= PRIORITY_LOOT_VISIT_STASH || saiph->world->player.hallucinating || saiph->world->player.blind || saiph->world->player.encumbrance > UNENCUMBERED)
		return;
	/* go to "visit_old_stash" if it's set */
	if (visit_old_stash.level >= 0 && visit_old_stash.level < (int) saiph->levels.size()) {
		map<Point, Stash>::iterator s = saiph->levels[visit_old_stash.level].stashes.find(visit_old_stash);
		if (s != saiph->levels[visit_old_stash.level].stashes.end()) {
			int moves = 0;
			unsigned char dir = saiph->shortestPath(visit_old_stash, false, &moves);
			if (dir != NOWHERE && dir != ILLEGAL_DIRECTION) {
				/* move towards stash */
				command = dir;
				priority = PRIORITY_LOOT_VISIT_STASH;
				return;
			}
		}
	}
	if (symbol != STAIRS_DOWN)
		return;
	/* set "visit_old_stash" when we're standing on downstairs */
	visitOldStash();
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
		if (saiph->inventory.size() < KNAPSACK_LIMIT) {
			/* we should have room for it in the knapsack */
			for (map<unsigned char, Item>::iterator p = saiph->pickup.begin(); p != saiph->pickup.end(); ++p) {
				int wanted = pickupItem(p->second);
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
		}
		/* if we're here, we should get next page or close list */
		showing_pickup = true;
		command = CLOSE_PAGE;
		priority = PRIORITY_CLOSE_PAGE;
	} else if (saiph->got_drop_menu) {
		showing_drop = true;
		if (saiph->getDungeonSymbol() == STAIRS_UP) {
			/* drop unwanted stuff */
			for (map<unsigned char, Item>::iterator d = saiph->drop.begin(); d != saiph->drop.end(); ++d) {
				if (d->second.name == "gold piece")
					continue; // don't drop gold
				int unwanted = dropItem(d->second);
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
			command = CLOSE_PAGE;
			priority = PRIORITY_CLOSE_PAGE;
		}
	} else if (saiph->world->menu && showing_inventory) {
		/* we should close the page of the inventory we're showing */
		command = CLOSE_PAGE;
		priority = PRIORITY_CLOSE_PAGE;
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

	if (messages.find(LOOT_STOLE, 0) != string::npos || messages.find(LOOT_STEALS, 0) != string::npos) {
		/* some monster stole something, we should check our inventory */
		checkInventory();
	} else if (messages.find(MESSAGE_DESTROY_POTION_FIRE, 0) != string::npos || messages.find(MESSAGE_DESTROY_POTION_FIRE2, 0) != string::npos || messages.find(MESSAGE_DESTROY_POTION_COLD, 0) != string::npos || messages.find(MESSAGE_DESTROY_POTION_COLD2, 0) != string::npos || messages.find(MESSAGE_DESTROY_RING, 0) != string::npos || messages.find(MESSAGE_DESTROY_RING2, 0) != string::npos || messages.find(MESSAGE_DESTROY_WAND, 0) != string::npos || messages.find(MESSAGE_DESTROY_WAND2, 0) != string::npos) {
		/* some of our inventory was destroyed. check it */
		checkInventory();
	} else if (messages.find(SAIPH_POLYMORPH, 0) != string::npos) {
		/* we polymorphed, check inventory */
		checkInventory();
	}

	if (saiph->world->question && messages.find(LOOT_NAME_INDIVIDUAL_OBECT, 0) == 0) {
		/* name or call an item */
		if (name_items.size() > 0)
			command = YES;
		else if (call_items.size() > 0)
			command = NO;
		else
			command = CLOSE_PAGE; // shouldn't happen
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (saiph->world->question && messages.find(LOOT_ITEM_TO_NAME, 0) == 0) {
		/* which item to name */
		map<unsigned char, string>::iterator n = name_items.begin();
		if (n == name_items.end())
			command = CLOSE_PAGE; // shouldn't happen
		else
			command = n->first;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (saiph->world->question && messages.find(LOOT_ITEM_TO_CALL, 0) == 0) {
		/* which item to call */
		map<unsigned char, string>::iterator c = call_items.begin();
		if (c == call_items.end())
			command = CLOSE_PAGE; // shouldn't happen
		else
			command = c->first;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (saiph->world->question && messages.find(LOOT_WHAT_TO_NAME_ITEM, 0) == 0) {
		/* what to name the item. we're gonna assume items aren't rearranged since last question */
		map<unsigned char, string>::iterator n = name_items.begin();
		if (n == name_items.end())
			command = CLOSE_PAGE; // *really* shouldn't happen
		else
			command = n->second;
		command.push_back('\n');
		priority = PRIORITY_CONTINUE_ACTION;
		/* it's extremely unlikely this last command fails, so remove it from name_items */
		name_items.erase(n);
		/* and check inventory */
		checkInventory();
	} else if (saiph->world->question && messages.find(LOOT_WHAT_TO_CALL_ITEM, 0) == 0) {
		/* what to call the item. we're gonna assume items aren't rearranged since last question */
		map<unsigned char, string>::iterator c = call_items.begin();
		if (c == call_items.end())
			command = CLOSE_PAGE; // *really* shouldn't happen
		else
			command = c->second;
		command.push_back('\n');
		priority = PRIORITY_CONTINUE_ACTION;
		/* it's extremely unlikely this last command fails, so remove it from name_items */
		call_items.erase(c);
		/* and check inventory */
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
		groups[request.key].amount = request.value;
		return true;
	} else if (request.request == REQUEST_ITEM_GROUP_ADD) {
		/* add item to group */
		if (items.find(request.data) == items.end()) {
			/* we need to add an entry about this item */
			items[request.data].amount = 0;
			items[request.data].beatitude = request.beatitude;
			items[request.data].only_unknown_enchantment = false;
		}
		groups[request.key].items.push_back(request.data);
		return true;
	} else if (request.request == REQUEST_ITEM_PICKUP) {
		/* pick up items that are not part of a group */
		items[request.data].amount = request.value;
		items[request.data].beatitude = request.beatitude;
		items[request.data].only_unknown_enchantment = request.only_unknown_enchantment;
		return true;
	} else if (request.request == REQUEST_CALL_ITEM) {
		call_items[request.key] = request.data;
		if (priority < PRIORITY_LOOK) {
			command = NAME;
			priority = PRIORITY_LOOK;
		}
	} else if (request.request == REQUEST_NAME_ITEM) {
		name_items[request.key] = request.data;
		if (priority < PRIORITY_LOOK) {
			command = NAME;
			priority = PRIORITY_LOOK;
		}
	}
	return false;
}

/* private methods */
void Loot::checkInventory() {
	dirty_inventory = true;
	if (priority >= PRIORITY_LOOK)
		return;
	command = INVENTORY;
	priority = PRIORITY_LOOK;
}

void Loot::checkStash() {
	dirty_stash = true;
	if (priority >= PRIORITY_LOOK)
		return;
	command = LOOK;
	priority = PRIORITY_LOOK;
}

int Loot::dropItem(const Item &item) {
	/* return how many items of this type should be dropped */
	if (!item.additional.empty())
		return 0; // hack: don't drop anything that got additional data ("wielded", "being worn", etc)
	if (item.name.find("(", 0) != string::npos)
		return 0; // even more of a hack; thoroughly rotted thoroughly burned iron helm (being wo
	return pickupOrDropItem(item, true);
}

int Loot::pickupItem(const Item &item) {
	/* return how many items of this type should be picked up */
	return pickupOrDropItem(item, false);
}

int Loot::pickupOrDropItem(const Item &item, bool drop) {
	/* helper method for dropItem() and pickupItem() */
	map<string, ItemWanted>::iterator i = items.find(item.name);
	if (i == items.end())
		return (drop ? item.count : 0); // item is not in our list
	if ((item.beatitude & i->second.beatitude) == 0)
		return (drop ? item.count : 0); // item does not have a beatitude we'll accept
	if (item.name == DISCARD)
		return (drop ? item.count : 0); // item is named "discard", we don't want the item
	/* groups */
	for (map<unsigned char, ItemGroup>::iterator g = groups.begin(); g != groups.end(); ++g) {
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
			return (drop ? 0 : g->second.amount - count);
		else
			return (drop ? count - g->second.amount : 0);
	}
	/* solitary items */
	if (i->second.amount <= 0)
		return (drop ? item.count : 0); // we don't desire this item
	if (i->second.only_unknown_enchantment && !item.unknown_enchantment)
		return (drop ? item.count : 0); // we only want this item if it got unknown enchantment, this is not the case
	/* figure out how many we got of this item already */
	int count = 0;
	for (map<unsigned char, Item>::iterator in = saiph->inventory.begin(); in != saiph->inventory.end(); ++in) {
		if (in->second.name != item.name)
			continue;
		count += in->second.count;
	}
	if (count <= i->second.amount)
		return (drop ? 0 : i->second.amount - count);
	else
		return (drop ? count - i->second.amount : 0);
}

void Loot::visitOldStash() {
	int min_moves = INT_MAX;
	visit_old_stash.level = -1; // reset, in case there are no old stashes we wish to visit
	for (vector<Level>::size_type level = 0; level < saiph->levels.size(); ++level) {
		for (map<Point, Stash>::iterator s = saiph->levels[level].stashes.begin(); s != saiph->levels[level].stashes.end(); ++s) {
			Coordinate stash(level, s->first);
			map<Coordinate, int>::iterator v = visit_stash.find(stash);
			if (v != visit_stash.end() && v->second == s->second.turn_changed) {
				/* stash is unchanged, but does it contain something nifty? */
				if (saiph->getDungeonSymbol(v->first) != SHOP_TILE) {
					for (list<Item>::iterator i = s->second.items.begin(); i != s->second.items.end(); ++i) {
						if (pickupItem(*i) == 0)
							continue; // don't want this item
						// we want this item, is stash closer than previous stash?
						int moves = 0;
						unsigned char dir = saiph->shortestPath(stash, false, &moves);
						if (dir != NOWHERE && dir != ILLEGAL_DIRECTION && moves < min_moves) {
							// move towards stash
							min_moves = moves;
							command = dir;
							priority = PRIORITY_LOOT_VISIT_STASH;
							visit_old_stash = stash;
						}
					}
				}
			} else {
				/* unvisited stash, visit it if it's closer */
				/* what? isn't this already covered?
				 * actually, no. this one cares about unvisited stashes on other levels too */
				int moves = 0;
				unsigned char dir = saiph->shortestPath(stash, false, &moves);
				if (dir != NOWHERE && dir != ILLEGAL_DIRECTION && moves < min_moves) {
					/* move towards stash */
					min_moves = moves;
					command = dir;
					priority = PRIORITY_LOOT_VISIT_STASH;
					visit_old_stash = stash;
				}
			}
		}
	}
}
