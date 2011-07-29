#include "Analyzers/Amulet.h"

#include <stdlib.h>
#include "Debug.h"
#include "EventBus.h"
#include "Globals.h"
#include "Inventory.h"
#include "Item.h"
#include "World.h"
#include "Actions/Call.h"
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
	if (!Saiph::polymorphed() && _amulet_key != ILLEGAL_ITEM && _amulet_key != Inventory::keyForSlot(SLOT_AMULET) && Inventory::keyForSlot(SLOT_AMULET) == ILLEGAL_ITEM) {
		World::setAction(static_cast<action::Action*> (new action::PutOn(this, _amulet_key, PRIORITY_AMULET_WEAR)));
	}
}

void Amulet::parseMessages(const string& messages) {
	if (messages.find(STRANGULATION_5TTL) != string::npos) {
		World::setAction(static_cast<action::Action*> (new action::Remove(this, _amulet_key, PRIORITY_AMULET_REMOVE_HARM)));
	} else if (messages.find(RESTFUL_SLEEP) != string::npos) {
		World::queueAction(static_cast<action::Action*> (new action::Call(this, _amulet_key, "restful sleep")));
		World::setAction(static_cast<action::Action*> (new action::Remove(this, _amulet_key, PRIORITY_AMULET_REMOVE_HARM)));
	}
}

/* methods */
void Amulet::onEvent(Event* const event) {
	if (event->id() == ChangedInventoryItems::ID) {
		ChangedInventoryItems* e = static_cast<ChangedInventoryItems*> (event);
		map<unsigned char, Item> k;
		for (set<unsigned char>::iterator i = e->keys().begin(); i != e->keys().end(); ++i) {
			Item item = Inventory::itemAtKey(*i);
			k.insert(pair<unsigned char, Item>(*i, item));
		}
		_amulet_key = bestAmulet(k);
	} else if (event->id() == ReceivedItems::ID) {
		ReceivedItems* e = static_cast<ReceivedItems*> (event);
		_amulet_key = bestAmulet(e->items());
	} else if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (wantItem(i->second))
				i->second.want(i->second.count());
		}
	}
}

/* private methods */
bool Amulet::wantItem(const Item& item) {
	if (item.name().find("strangulation") != string::npos || item.name().find("restful") != string::npos)
		return false;
	if (item.beatitude() == CURSED)
		return false;
	return data::Amulet::amulets().find(item.name()) != data::Amulet::amulets().end();
}

unsigned char Amulet::bestAmulet(const map<unsigned char, Item>& keys) {
	Item suit = Inventory::itemInSlot(SLOT_SUIT);
	Item shield = Inventory::itemInSlot(SLOT_SHIELD);
	bool reflection = suit.name().find("silver dragon") != string::npos || shield.name().find("silver shield") != string::npos || shield.name().find("reflection") != string::npos;
	unsigned char best = ILLEGAL_ITEM;
	for (map<unsigned char, Item>::const_iterator i = keys.begin(); i != keys.end(); ++i) {
		map<const string, const data::Amulet*>::const_iterator a = data::Amulet::amulets().find(i->second.name());
		if (a == data::Amulet::amulets().end())
			continue;
		if (i->second.beatitude() == CURSED)
			continue;
		if (i->second.beatitude() == BEATITUDE_UNKNOWN) {
			Beatify b(i->first, 200);
			EventBus::broadcast(&b);
			continue;
		}
		if ((i->second.name().find("restful") != string::npos) || i->second.name().find("strangulation") != string::npos)
			continue;
		// priorities: reflection > life saving > unknown > unchanging > ESP
		if (!reflection && i->second.name().find("reflection") != string::npos)
			return i->first; // amulet of reflection and no reflection from armor, break loop and return this amulet
		if (i->second.name().find("life saving") != string::npos)
			best = i->first; // life saving amulet is best
		else if (i->second.name().find("unchanging") != string::npos && (best == ILLEGAL_ITEM || Inventory::itemAtKey(best).name().find("ESP") != string::npos))
			best = i->first; // found unchanging amulet and currently best amulet is esp, replace best
		else if (best == ILLEGAL_ITEM && i->second.name().find("ESP") != string::npos)
			best = i->first; // found esp amulet and no other amulets are found, set best
		else if (best == ILLEGAL_ITEM || Inventory::itemAtKey(best).name().find("life saving") == string::npos)
			best = i->first; // found unknown amulet and haven't found life saving amulet, replace best
	}
	return best;
}
