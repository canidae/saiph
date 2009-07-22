#include <sstream>
#include <stdlib.h>
#include "Loot.h"
#include "../Debug.h"
#include "../EventBus.h"
#include "../Inventory.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Actions/ListInventory.h"
#include "../Actions/Look.h"
#include "../Actions/Move.h"
#include "../Actions/SelectMultiple.h"
#include "../Events/StashChanged.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Loot::Loot() : Analyzer("Loot"), showing_pickup(false), showing_drop(false) {
	/* register events */
	EventBus::registerEvent(StashChanged::id, this);
}

/* methods */
void Loot::analyze() {
	/* check inventory if it's not updated */
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
			if (World::setAction(static_cast<action::Action *>(new action::Look(this)))) {
				visit.erase(v++);
				continue;
			}
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
		/* reset WantItems lists */
		wi.items.clear();
		wi.want.clear();
		while (pos != string::npos && messages.size() > pos + 6) {
			pos += 6;
			string::size_type length = messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			if (messages[pos - 2] == '-') {
				wi.items[messages[pos - 4]] = Item(messages.substr(pos, length));
				wi.want[messages[pos - 4]] = 0;
			}
			pos += length;
		}
		/* broadcast event */
		EventBus::broadcast(static_cast<Event *>(&wi));
		/* pick up stuff that was wanted by analyzers */
		vector<string> pickup;
		ostringstream tmp;
		for (map<unsigned char, int>::iterator w = wi.want.begin(); w != wi.want.end(); ++w) {
			if (w->second <= 0)
				continue;
			tmp.str("");
			tmp << w->second << w->first;
			pickup.push_back(tmp.str());
		}
		World::setAction(static_cast<action::Action *>(new action::SelectMultiple(this, pickup)));
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
		/* reset WantItems lists */
		wi.items.clear();
		wi.want.clear();
		while (pos != string::npos && messages.size() > pos + 6) {
			pos += 6;
			string::size_type length = messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			if (messages[pos - 2] == '-') {
				map<unsigned char, Item>::iterator i = Inventory::items.find(messages[pos - 4]);
				if (i != Inventory::items.end()) {
					wi.items[messages[pos - 4]] = i->second;
					/* let's also pretend we don't have any examples of the item in our inventory */
					i->second.count = 0;
				} else {
					/* this isn't supposed to happen (inventory not updated?) */
					wi.items[messages[pos - 4]] = Item(messages.substr(pos, length));
				}
				wi.want[messages[pos - 4]] = 0;
			}
			pos += length;
		}
		/* broadcast event */
		EventBus::broadcast(static_cast<Event *>(&wi));
		/* drop stuff no analyzer wanted */
		vector<string> drop;
		ostringstream tmp;
		for (map<unsigned char, int>::iterator w = wi.want.begin(); w != wi.want.end(); ++w) {
			if (w->second <= 0) {
				/* drop for beatitude or we don't want the item */
				drop.push_back(std::string(1, w->first));
				continue;
			}
			map<unsigned char, Item>::iterator i = wi.items.find(w->first);
			if (i == wi.items.end()) {
				/* what? no, this shouldn't happen */
				continue;
			} else if (i->second.count <= w->second) {
				/* we want all of these items, don't drop them */
				continue;
			}
			/* drop some of these items */
			tmp.str("");
			tmp << (i->second.count - w->second) << w->first;
			drop.push_back(tmp.str());
		}
		World::setAction(static_cast<action::Action *>(new action::SelectMultiple(this, drop)));
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
