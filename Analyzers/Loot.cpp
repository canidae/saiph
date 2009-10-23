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
Loot::Loot() : Analyzer("Loot"), _showing_pickup(false), _showing_drop(false) {
	/* register events */
	EventBus::registerEvent(StashChanged::ID, this);
}

/* methods */
void Loot::analyze() {
	/* check inventory if it's not updated */
	if (!Inventory::updated()) {
		World::setAction(static_cast<action::Action*> (new action::ListInventory(this)));
		return;
	}

	/* visit new/changed stashes */
	set<Coordinate>::iterator v = _visit.begin();
	while (v != _visit.end()) {
		map<Point, Stash>::iterator s = World::level(Saiph::position().level()).stashes().find(*v);
		if (s == World::level(Saiph::position().level()).stashes().end() || s->second.lastInspected() == World::turn()) {
			/* stash is gone or we recently looked at it */
			_visit.erase(v++);
			continue;
		}
		const Tile& tile = World::shortestPath(*v);
		if (tile.direction() == NOWHERE) {
			/* standing on stash, look and remove from visit */
			if (World::setAction(static_cast<action::Action*> (new action::Look(this)))) {
				_visit.erase(v++);
				continue;
			}
		} else if (tile.cost() < UNPASSABLE) {
			/* move to stash */
			World::setAction(static_cast<action::Action*> (new action::Move(this, tile.direction(), action::Move::calculatePriority(PRIORITY_LOOT_VISIT, tile.distance()))));
		}
		++v;
	}
}

void Loot::parseMessages(const string& messages) {
	string::size_type pos;

	if (!World::menu()) {
		_showing_pickup = false;
		_showing_drop = false;
	} else if ((pos = messages.find(MESSAGE_PICK_UP_WHAT)) != string::npos || _showing_pickup) {
		/* picking up stuff */
		if (_showing_pickup) {
			/* not the first page, set pos to 0 */
			pos = 0;
		} else {
			/* first page */
			_showing_pickup = true;
			/* and find first "  " */
			pos = messages.find("  ", pos + 1);
		}
		/* reset WantItems lists */
		_wi.clear();
		while (pos != string::npos && messages.size() > pos + 6) {
			pos += 6;
			string::size_type length = messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			if (messages[pos - 2] == '-')
				_wi.addItem(messages[pos - 4], Item(messages.substr(pos, length), 0));
			pos += length;
		}
		/* broadcast event */
		EventBus::broadcast(static_cast<Event*> (&_wi));
		/* pick up stuff that was wanted by analyzers */
		vector<string> pickup;
		ostringstream tmp;
		for (map<unsigned char, Item>::iterator i = _wi.items().begin(); i != _wi.items().end(); ++i) {
			if (i->second.want() <= 0)
				continue;
			tmp.str("");
			tmp << i->second.want() << i->first;
			pickup.push_back(tmp.str());
		}
		World::setAction(static_cast<action::Action*> (new action::SelectMultiple(this, pickup)));
	} else if ((pos = messages.find(MESSAGE_DROP_WHICH_ITEMS)) != string::npos || _showing_drop) {
		/* dropping items */
		if (_showing_drop) {
			/* not the first page, set pos to 0 */
			pos = 0;
		} else {
			/* first page, set menu */
			_showing_drop = true;
			;
			/* and find first "  " */
			pos = messages.find("  ", pos + 1);
		}
		/* reset WantItems lists */
		_wi.clear();
		while (pos != string::npos && messages.size() > pos + 6) {
			pos += 6;
			string::size_type length = messages.find("  ", pos);
			if (length == string::npos)
				break;
			length = length - pos;
			if (messages[pos - 2] == '-') {
				map<unsigned char, Item>::iterator i = Inventory::items().find(messages[pos - 4]);
				if (i != Inventory::items().end()) {
					i->second.want(0);
					_wi.addItem(messages[pos - 4], i->second);
					/* let's also pretend we don't have any examples of the item in our inventory */
					i->second.count(0);
				} else {
					/* this isn't supposed to happen (inventory not updated?) */
					_wi.addItem(messages[pos - 4], Item(messages.substr(pos, length), 0));
				}
			}
			pos += length;
		}
		/* broadcast event */
		EventBus::broadcast(static_cast<Event*> (&_wi));
		/* drop stuff no analyzer wanted */
		vector<string> drop;
		ostringstream tmp;
		for (map<unsigned char, Item>::iterator i = _wi.items().begin(); i != _wi.items().end(); ++i) {
			if (i->second.want() <= 0) {
				/* drop for beatitude or we don't want the item */
				drop.push_back(std::string(1, i->first));
				continue;
			}
			if (i->second.count() <= i->second.want()) {
				/* we want all of these items, don't drop them */
				continue;
			}
			/* drop some of these items */
			tmp.str("");
			tmp << (i->second.count() - i->second.want()) << i->first;
			drop.push_back(tmp.str());
		}
		World::setAction(static_cast<action::Action*> (new action::SelectMultiple(this, drop)));
	}

	if (messages.find(MESSAGE_SEVERAL_OBJECTS_HERE) != string::npos || messages.find(MESSAGE_MANY_OBJECTS_HERE) != string::npos || messages.find(MESSAGE_SEVERAL_MORE_OBJECTS_HERE) != string::npos || messages.find(MESSAGE_MANY_MORE_OBJECTS_HERE) != string::npos) {
		/* several/many objects herek, take a look around */
		World::setAction(static_cast<action::Action*> (new action::Look(this)));
	}
}

void Loot::onEvent(Event * const event) {
	if (event->id() == StashChanged::ID) {
		/* stash changed, we need to visit it again */
		StashChanged* e = static_cast<StashChanged*> (event);
		_visit.insert(e->stash());
	}
}
