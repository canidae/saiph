#include <stdlib.h>
#include "Loot.h"
#include "../EventBus.h"
#include "../Inventory.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Actions/ListInventory.h"
#include "../Actions/Look.h"
#include "../Actions/Move.h"
#include "../Events/StashChanged.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Loot::Loot() : Analyzer("Loot"), showing_pickup(false), showing_drop(false) {
}

/* methods */
void Loot::analyze() {
	/* check inventory if it's not updated */
	/* TODO: we can make the analyzers that intend to use inventory items check this before they use the item */
	if (!Inventory::updated) {
		World::setAction(static_cast<action::Action *>(new action::ListInventory(this)));
		return;
	}

	/* visit new/changed stashes */
	set<Coordinate>::iterator v = visit.begin();
	while (v != visit.end()) {
		const PathNode &node = World::shortestPath(*v);
		if (node.dir == NOWHERE) {
			/* standing on stash, look and remove from visit */
			if (World::setAction(static_cast<action::Action *>(new action::Look(this))))
				visit.erase(v++);
			continue;
		} else if (node.cost < UNPASSABLE) {
			/* move to stash */
			World::setAction(static_cast<action::Action *>(new action::Move(this, node.dir, action::Move::calculatePriority(PRIORITY_LOOT_VISIT, node.moves))));
		}
		++v;
	}
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

void Loot::onEvent(Event *const event) {
	if (event->getID() == StashChanged::id) {
		/* stash changed, we need to visit it again */
		StashChanged *e = static_cast<StashChanged *>(event);
		visit.insert(e->stash);
	}
}
