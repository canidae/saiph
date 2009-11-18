#include "Loot.h"

#include <sstream>
#include <stdlib.h>
#include "../Debug.h"
#include "../EventBus.h"
#include "../Inventory.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Actions/ListInventory.h"
#include "../Actions/Look.h"
#include "../Actions/Loot.h"
#include "../Actions/Move.h"
#include "../Events/StashChanged.h"
#include "../Events/ItemsOnGround.h"
#include "../Events/WantItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Loot::Loot() : Analyzer("Loot") {
	/* register events */
	EventBus::registerEvent(StashChanged::ID, this);
	EventBus::registerEvent(ItemsOnGround::ID, this);
}

/* methods */
void Loot::analyze() {
	/* check inventory if it's not updated */
	if (!Inventory::updated()) {
		World::setAction(static_cast<action::Action*> (new action::ListInventory(this)));
		return;
	}

	/* don't move around when blind/confused/stunned */
	if (Saiph::blind() || Saiph::confused() || Saiph::stunned())
		return;

	/* visit new/changed stashes */
	set<Coordinate>::iterator v = _visit.begin();
	while (v != _visit.end()) {
		map<Point, Stash>::iterator s = World::level(Saiph::position().level()).stashes().find(*v);
		if (s == World::level(Saiph::position().level()).stashes().end() || s->second.lastInspected() == World::turn()) {
			/* stash is gone or we recently looked at it */
			_visit.erase(v++);
			continue;
		}
		Tile tile = World::shortestPath(*v);
		if (tile.direction() == NOWHERE) {
			/* standing on stash, look and remove from visit */
			if (World::setAction(static_cast<action::Action*> (new action::Look(this)))) {
				_visit.erase(v++);
				continue;
			}
		} else if (tile.cost() < UNPASSABLE) {
			/* move to stash */
			World::setAction(static_cast<action::Action*> (new action::Move(this, tile.direction(), action::Move::calculatePriority(PRIORITY_LOOT_VISIT, tile.cost()))));
		}
		++v;
	}
}

void Loot::parseMessages(const string& messages) {
	if (messages.find(MESSAGE_SEVERAL_OBJECTS_HERE) != string::npos || messages.find(MESSAGE_MANY_OBJECTS_HERE) != string::npos || messages.find(MESSAGE_SEVERAL_MORE_OBJECTS_HERE) != string::npos || messages.find(MESSAGE_MANY_MORE_OBJECTS_HERE) != string::npos) {
		/* several/many objects herek, take a look around */
		World::setAction(static_cast<action::Action*> (new action::Look(this)));
	}
}

void Loot::onEvent(Event * const event) {
	if (event->id() == StashChanged::ID) {
		/* stash changed, we need to visit it again */
		StashChanged* e = static_cast<StashChanged*> (event);
		_visit.insert(e->position());
	} else if (event->id() == ItemsOnGround::ID) {
		/* standing on stash, ask if anyone want anything */
		// TODO: proper shopping code
		if (World::level().tile().symbol() != SHOP_TILE) {
			ItemsOnGround* e = static_cast<ItemsOnGround*> (event);
			int index = 0;
			bool looting = false;
			list<Item>::const_iterator i = e->items().begin();
			/* if we're standing on STAIRS_UP we'll pretend it's a safe stash */
			bool safe_stash = World::level().tile().symbol() == STAIRS_UP;
			WantItems wi(false, safe_stash);
			while (!looting) {
				wi.addItem(index++, *i);
				++i;
				if (index == UCHAR_MAX || i == e->items().end()) {
					EventBus::broadcast(static_cast<Event*> (&wi));
					for (map<unsigned char, Item>::iterator i = wi.items().begin(); i != wi.items().end(); ++i) {
						if (i->second.want() <= 0)
							continue;
						/* someone want an item in this stash */
						World::setAction(static_cast<action::Action*> (new action::Loot(this, PRIORITY_LOOT, safe_stash)));
						looting = true;
						break;
					}
					wi.clear();
					index = 0;
				}
				if (i == e->items().end())
					break;
			}
		}
	}
}
