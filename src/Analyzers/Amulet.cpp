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
		_amulet_key = bestAmulet((e->items()));
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
	unsigned char reflection, life_saving, unchanging, unknown, ESP = ILLEGAL_ITEM;
	reflection = life_saving = unchanging = unknown = ESP = ILLEGAL_ITEM;
	for (map<unsigned char, Item>::const_iterator i = keys.begin(); i != keys.end(); ++i) {
		map<const string, const data::Amulet*>::const_iterator a = data::Amulet::amulets().find(i->second.name());
		if (a == data::Amulet::amulets().end())
			continue;
		if (i->second.beatitude() == CURSED)
			continue;
		if (i->second.beatitude() == BEATITUDE_UNKNOWN) {
			Beatify b(i->first, 900);
			EventBus::broadcast(&b);
			continue;
		}
		if ((i->second.name().find("restful") != string::npos) || i->second.name().find("strangulation") != string::npos)
			continue;
		if (i->second.name().find("reflection") != string::npos)
			reflection = i->first;
		else if (i->second.name().find("life saving") != string::npos)
			life_saving = i->first;
		else if (i->second.name().find("unchanging") != string::npos)
			unchanging = i->first;
		else if (i->second.name().find("ESP") != string::npos)
			ESP = i->first;
		unknown = i->first;
	}
	if (unchanging == ILLEGAL_ITEM)
		unchanging = unknown;
	Debug::info() << "refl: " << reflection << " ls : " << life_saving << endl;
	// priorities: reflection > life saving > unchanging > ESP
	if (wearing("silver dragon scale mail") || wearing("silver dragon scales"))
		return (life_saving != ILLEGAL_ITEM) ? life_saving : (unchanging != ILLEGAL_ITEM) ? unchanging : ESP;
	else
		return (reflection != ILLEGAL_ITEM) ? reflection : (life_saving != ILLEGAL_ITEM) ? life_saving : unchanging;
}

bool Amulet::wearing(const string& name) {
	for (map<unsigned char, Item>::iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i) {
		if (i->second.name().find(name) != string::npos)
			return true;
	}
	return false;
}
