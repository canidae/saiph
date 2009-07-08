#include <stdlib.h>
#include "Loot.h"
#include "../EventBus.h"
#include "../Inventory.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Actions/ListInventory.h"
#include "../Actions/Look.h"

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

	/* TODO: visit new/changed stashes, using event TakeMeThere */
}

void Loot::parseMessages(const string &messages) {
	string::size_type pos;

	if (!World::menu) {
		showing_pickup = false;
		showing_drop = false;
	} else if ((pos = messages.find(MESSAGE_PICK_UP_WHAT)) != string::npos || showing_pickup) {
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
	} else if ((pos = messages.find(MESSAGE_DROP_WHICH_ITEMS)) != string::npos || showing_drop) {
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
		if (Inventory::items.size() < KNAPSACK_LIMIT) {
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
				Inventory::items[d->first].count = 0;
				command = d->first;
			} else {
				/* drop some */
				Inventory::items[d->first].count -= unwanted;
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

	if (messages.find(LOOT_SEVERAL_OBJECTS_HERE) != string::npos || messages.find(LOOT_MANY_OBJECTS_HERE) != string::npos || messages.find(LOOT_SEVERAL_MORE_OBJECTS_HERE) != string::npos || messages.find(LOOT_MANY_MORE_OBJECTS_HERE) != string::npos) {
		/* several/many objects herek, take a look around */
		World::setAction(static_cast<action::Action *>(new action::Look(this)));
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
	if (Saiph::hallucinating || Saiph::blind || Saiph::encumbrance > UNENCUMBERED || Inventory::items.size() >= KNAPSACK_LIMIT)
		return;
	unsigned int min_moves = UNREACHABLE;
	visit_old_stash.level = -1; // reset, in case there are no old stashes we wish to visit
	for (vector<Level>::size_type level = 0; level < saiph->levels.size(); ++level) {
		for (map<Point, Stash>::iterator s = saiph->levels[level].stashes.begin(); s != saiph->levels[level].stashes.end(); ++s) {
			Coordinate stash(level, s->first);
			map<Coordinate, int>::iterator v = visit_stash.find(stash);
			if (v != visit_stash.end() && v->second == s->second.turn_changed) {
				/* stash is unchanged, but does it contain something nifty? */
				if (World::getDungeonSymbol(v->first) != SHOP_TILE) {
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
