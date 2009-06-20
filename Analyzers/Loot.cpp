#include <stdlib.h>
#include "Loot.h"
#include "../EventBus.h"
#include "../Inventory.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Actions/ListInventory.h"

using namespace analyzer;
using namespace std;

/* constructors/destructor */
Loot::Loot() : Analyzer("Loot"), showing_pickup(false), showing_drop(false) {
}

/* methods */
void Loot::analyze() {
	/* check inventory if it's not updated */
	if (!Inventory::updated) {
		World::setAction(static_cast<action::Action *>(new action::ListInventory(this)));
		return;
	}

	/* loot stash we're standing on unless full knapsack or burdened */
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
	unsigned int min_moves = UNREACHABLE;
	for (map<Point, Stash>::iterator s = saiph->levels[saiph->position.level].stashes.begin(); s != saiph->levels[saiph->position.level].stashes.end(); ++s) {
		map<Coordinate, int>::iterator v = visit_stash.find(Coordinate(saiph->position.level, s->first));
		if (v != visit_stash.end() && v->second == s->second.turn_changed)
			continue; // stash is unchanged
		/* unvisited stash, visit it if it's closer */
		const PathNode &node = saiph->shortestPath(s->first);
		if (node.dir != NOWHERE && node.cost < UNPASSABLE && node.moves < min_moves) {
			/* move towards stash */
			min_moves = node.moves;
			command = node.dir;
			priority = PRIORITY_LOOT_VISIT_STASH;
		}
	}

	if (priority >= PRIORITY_LOOT_VISIT_STASH || saiph->world->player.hallucinating || saiph->world->player.blind || saiph->world->player.encumbrance > UNENCUMBERED || saiph->inventory.size() >= KNAPSACK_LIMIT)
		return;
	/* go to "visit_old_stash" if it's set */
	if (visit_old_stash.level >= 0 && visit_old_stash.level < (int) saiph->levels.size()) {
		map<Point, Stash>::iterator s = saiph->levels[visit_old_stash.level].stashes.find(visit_old_stash);
		if (s != saiph->levels[visit_old_stash.level].stashes.end()) {
			const PathNode &node = saiph->shortestPath(visit_old_stash);
			if (node.dir != NOWHERE && node.cost < UNPASSABLE) {
				/* move towards stash */
				command = node.dir;
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

void Loot::parseMessages(const string &messages) {
	string::size_type pos;

	if (!World::menu) {
		showing_pickup = false;
		showing_drop = false;
	} else if ((pos = messages.find(MESSAGE_PICK_UP_WHAT, 0)) != string::npos || showing_pickup) {
		/* picking up stuff */
		if (showing_pickup) {
			/* not the first page, set pos to 0 */
			pos = 0;
		} else {
			/* first page */
			showing_pickup = true;
			/* and find first "  " */
			pos = messages.find("  ", pos + 1);
		}
		/* reset pickup list */
		pickup.items.clear();
		while (pos != string::npos && messages.size() > pos + 6) {
			pos += 6;
			string::size_type length = messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			if (messages[pos - 2] == '-')
				pickup.items[messages[pos - 4]] = Item(messages.substr(pos, length));
			pos += length;
		}
		/* broadcast event */
		EventBus::broadcast(static_cast<Event *>(&pickup));
	} else if ((pos = messages.find(MESSAGE_DROP_WHICH_ITEMS, 0)) != string::npos || showing_drop) {
		/* dropping items */
		if (showing_drop) {
			/* not the first page, set pos to 0 */
			pos = 0;
		} else {
			/* first page, set menu */
			showing_drop = true;;
			/* and find first "  " */
			pos = messages.find("  ", pos + 1);
		}
		/* reset drop list */
		drop.keys.clear();
		while (pos != string::npos && messages.size() > pos + 6) {
			pos += 6;
			string::size_type length = messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			if (messages[pos - 2] == '-')
				drop.keys.insert(messages[pos - 4]);
			pos += length;
		}
		/* broadcast event */
		EventBus::broadcast(static_cast<Event *>(&drop));
	}

	if (saiph->showing_pickup) {
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
	} else if (saiph->showing_drop) {
		showing_drop = true;
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
void Loot::checkOldStash() {
	if (saiph->world->player.hallucinating || saiph->world->player.blind || saiph->world->player.encumbrance > UNENCUMBERED || saiph->inventory.size() >= KNAPSACK_LIMIT)
		return;
	unsigned int min_moves = UNREACHABLE;
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
						const PathNode &node = saiph->shortestPath(stash);
						if (node.dir != NOWHERE && node.cost < UNPASSABLE && node.moves < min_moves) {
							// move towards stash
							min_moves = node.moves;
							command = node.dir;
							priority = PRIORITY_LOOT_VISIT_STASH;
							visit_old_stash = stash;
						}
					}
				}
			} else {
				/* unvisited stash, visit it if it's closer */
				/* what? isn't this already covered?
				 * actually, no. this one cares about unvisited stashes on other levels too */
				const PathNode &node = saiph->shortestPath(stash);
				if (node.dir != NOWHERE && node.cost < UNPASSABLE && node.moves < min_moves) {
					/* move towards stash */
					min_moves = node.moves;
					command = node.dir;
					priority = PRIORITY_LOOT_VISIT_STASH;
					visit_old_stash = stash;
				}
			}
		}
	}
}
