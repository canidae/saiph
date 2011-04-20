#include "Analyzers/Loot.h"

#include <sstream>
#include <stdlib.h>
#include "Debug.h"
#include "EventBus.h"
#include "Inventory.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Drop.h"
#include "Actions/Engrave.h"
#include "Actions/ListInventory.h"
#include "Actions/Look.h"
#include "Actions/Loot.h"
#include "Actions/Move.h"
#include "Events/ElberethQuery.h"
#include "Events/StashChanged.h"
#include "Events/ItemsOnGround.h"
#include "Events/WantItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Loot::Loot() : Analyzer("Loot"), _visit() {
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

	/* don't move around when blind/confused/stunned/hallucinating */
	if (Saiph::blind() || Saiph::confused() || Saiph::stunned() || Saiph::hallucinating())
		return;

	/* visit new/changed stashes */
	set<Coordinate>::iterator v = _visit.begin();
	while (v != _visit.end()) {
		map<Point, Stash>::const_iterator s = World::level().stashes().find(*v);
		if (s == World::level().stashes().end() || s->second.lastInspected() == World::turn()) {
			/* stash is gone or we recently looked at it */
			_visit.erase(v++);
			continue;
		}
		Tile tile = World::shortestPath(*v);
		if (tile.direction() == NOWHERE) {
			/* standing on stash, look and remove from visit */
			if (World::setAction(static_cast<action::Action*> (new action::Look(this)))) {
				/* TODO: we may get something more important to do just as we step on the loot, making us move away from the stash. 
					 in that case she won't visit the stash again later. can't remove the stash from _visit here. */
				_visit.erase(v++);
				continue;
			}
		} else if (tile.cost() < UNPASSABLE) {
			/* move to stash */
			World::setAction(static_cast<action::Action*> (new action::Move(this, tile, action::Move::calculatePriority(PRIORITY_LOOT_VISIT, tile.cost()))));
		}
		++v;
	}

	if (World::level().tile().symbol() == STAIRS_UP) {
		/* stairs up, ask if anyone wants to drop anything and if they do, get down 3 E's and drop stuff */
		WantItems wi(true, true);
		for (map<unsigned char, Item>::iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i) {
			/* only add items not in a slot */
			if (Inventory::slotForKey(i->first) == ILLEGAL_SLOT)
				wi.addItem(i->first, i->second);
		}
		bool drop_on_stairs = false;
		EventBus::broadcast(static_cast<Event*> (&wi));
		for (map<unsigned char, Item>::iterator i = wi.items().begin(); !drop_on_stairs && i != wi.items().end(); ++i) {
			if (i->second.want() <= 0 || i->second.want() < i->second.count())
				drop_on_stairs = true;
		}
		if (drop_on_stairs) {
			ElberethQuery eq;
			EventBus::broadcast(static_cast<Event*> (&eq));
			if (eq.type() == ELBERETH_MUST_CHECK) {
				/* we don't know, we must look */
				World::setAction(static_cast<action::Action*> (new action::Look(this)));
			} else if (eq.type() == ELBERETH_DUSTED || eq.type() == ELBERETH_NONE) {
				/* no elbereth or dusted elbereth */
				if (eq.count() < 3) {
					/* less than 3 elbereths, engrave */
					World::setAction(static_cast<action::Action*> (new action::Engrave(this, ELBERETH "\n", HANDS, PRIORITY_LOOT_DROP, (eq.count() > 0))));
				} else {
					/* 3 or more elbereths, drop stuff we don't want */
					World::setAction(static_cast<action::Action*> (new action::Drop(this, PRIORITY_LOOT_DROP, true)));
				}
			}
		}
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
		if (World::level().tile().symbol() != SHOP_TILE && action::Loot::canLoot()) {
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
						if (i->second.want() <= 0 || i->second.count() <= 0)
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
