#include "Analyzers/Amulet.h"

#include "Debug.h"
#include <stdlib.h>
#include <string.h>
#include "EventBus.h"
#include "Globals.h"
#include "Inventory.h"
#include "Item.h"
#include "World.h"
#include "Actions/PutOn.h"
#include "Actions/Remove.h"
#include "Data/Amulet.h"
#include "Events/Beatify.h"
#include "Events/Event.h"
#include "Events/ChangedInventoryItems.h"
#include "Events/ReceivedItems.h"
#include "Events/WantItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Amulet::Amulet() : Analyzer("Amulet"), _amulet_key(ILLEGAL_ITEM) {
	/* register events */
	EventBus::registerEvent(ChangedInventoryItems::ID, this);
	EventBus::registerEvent(ReceivedItems::ID, this);
	EventBus::registerEvent(WantItems::ID, this);
}

void Amulet::analyze() {
	if (!Saiph::polymorphed() && _amulet_key != ILLEGAL_ITEM && _amulet_key != Inventory::keyForSlot(SLOT_AMULET)) {
		Debug::info() << "hello" << endl;
		World::setAction(static_cast<action::Action*> (new action::PutOn(this, _amulet_key, PRIORITY_AMULET_WEAR)));
	}
}

/* methods */
void Amulet::onEvent(Event * const event) {
	if (event->id() == ChangedInventoryItems::ID) {
		ChangedInventoryItems* e = static_cast<ChangedInventoryItems*> (event);
		for (set<unsigned char>::iterator i = e->keys().begin(); i != e->keys().end(); ++i) {
			Item item = Inventory::itemAtKey(*i);
			if (item.beatitude() != CURSED && item.beatitude() != BEATITUDE_UNKNOWN && data::Amulet::amulets().find(item.name()) != data::Amulet::amulets().end())
				_amulet_key = *i;
		}
		//todo: determine best amulet
	} else if (event->id() == ReceivedItems::ID) {
		ReceivedItems* e = static_cast<ReceivedItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (i->second.beatitude() != BEATITUDE_UNKNOWN || data::Amulet::amulets().find(i->second.name()) == data::Amulet::amulets().end())
				continue; // known beatitude or not an amulet
			else if (i->second.beatitude() == BEATITUDE_UNKNOWN && data::Amulet::amulets().find(i->second.name()) != data::Amulet::amulets().end()) {
				Beatify b(i->first, 175);
				EventBus::broadcast(&b);
			}
			else if (i->second.beatitude() != CURSED && data::Amulet::amulets().find(i->second.name()) != data::Amulet::amulets().end()) {
				_amulet_key = i->first;
				//TODO: detect if we want to actually wear it
			}
		}
		// FIXME
		//wearAmulet(e->items);
	} else if (event->id() == WantItems::ID) {
		Debug::info() << "WantItems appeared, wearing?" << endl;
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (wantItem(i->second))
				i->second.want(i->second.count());
		}
	}
}

/* private methods */
bool Amulet::wantItem(const Item& item) {
	return data::Amulet::amulets().find(item.name()) != data::Amulet::amulets().end();
}

void Amulet::wearAmulet(const set<unsigned char>& keys) {
	/* FIXME: need to make this smarter, so we can handle both ChangedInventoryItems and ReceivedItems events */
	if (Inventory::itemInSlot(SLOT_AMULET).beatitude() == CURSED)
		return; // wearing a cursed amulet, no point trying to wear another amulet

	/* find the best amulet */
	unsigned char best_key = Inventory::keyForSlot(SLOT_AMULET);

	for (set<unsigned char>::const_iterator k = keys.begin(); k != keys.end(); ++k) {
		map<unsigned char, Item>::iterator i = Inventory::items().find(*k);
		if (i == Inventory::items().end())
			return; // huh? this can't happen
		map<const string, const data::Amulet*>::const_iterator a = data::Amulet::amulets().find(i->second.name());
		if (a == data::Amulet::amulets().end())
			return; // this is no amulet

		/* TODO: is this amulet better than the one we wear? */
		/* TODO: if we find a better amulet, take off current amulet and return */
	}

	if (best_key == ILLEGAL_ITEM)
		return; // no new amulet to put on or wearing best amulet

	/* put on this amulet */
	//World::setAction(static_cast<action::Action*> (new action::PutOn(this, best_key, PRIORITY_AMULET_WEAR)));
}
