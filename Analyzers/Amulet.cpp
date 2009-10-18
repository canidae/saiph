#include "Amulet.h"
#include "../EventBus.h"
#include "../Globals.h"
#include "../Inventory.h"
#include "../Item.h"
#include "../World.h"
#include "../Actions/Loot.h"
#include "../Actions/PutOn.h"
#include "../Actions/Remove.h"
#include "../Actions/Select.h"
#include "../Data/Amulet.h"
#include "../Events/Event.h"
#include "../Events/ChangedInventoryItems.h"
#include "../Events/ItemsOnGround.h"
#include "../Events/ReceivedItems.h"
#include "../Events/WantItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Amulet::Amulet() : Analyzer("Amulet") {
	/* register events */
	EventBus::registerEvent(ChangedInventoryItems::ID, this);
	EventBus::registerEvent(ItemsOnGround::ID, this);
	EventBus::registerEvent(ReceivedItems::ID, this);
	EventBus::registerEvent(WantItems::ID, this);
}

/* methods */
void Amulet::onEvent(Event* const event) {
	if (event->id() == ChangedInventoryItems::ID) {
		ChangedInventoryItems* e = static_cast<ChangedInventoryItems*> (event);
		wearAmulet(e->keys());
	} else if (event->id() == ReceivedItems::ID) {
		// FIXME
		//ReceivedItems* e = static_cast<ReceivedItems*>(event);
		//wearAmulet(e->items);
	} else if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (wantItem(i->second))
				World::setAction(static_cast<action::Action*> (new action::Select(this, i->first)));
		}
	} else if (event->id() == ItemsOnGround::ID) {
		ItemsOnGround* e = static_cast<ItemsOnGround*> (event);
		for (list<Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (wantItem(*i))
				World::setAction(static_cast<action::Action*> (new action::Loot(this, PRIORITY_AMULET_LOOT)));
		}
	}
}

/* private methods */
bool Amulet::wantItem(const Item& item) {
	return data::Amulet::amulets.find(item.name) != data::Amulet::amulets.end();
}

void Amulet::wearAmulet(const set<unsigned char> &keys) {
	/* FIXME: need to make this smarter, so we can handle both ChangedInventoryItems and ReceivedItems events */
	map<unsigned char, Item>::iterator worn = Inventory::items().find(Inventory::itemInSlot(SLOT_AMULET));
	if (worn != Inventory::items().end() && worn->second.beatitude == CURSED)
		return; // wearing a cursed amulet, no point trying to wear another amulet

	/* find the best amulet */
	unsigned char best_key = (worn == Inventory::items().end()) ? '\0' : worn->first;
	map<string, data::Amulet*>::iterator best_amulet = (worn == Inventory::items().end()) ? data::Amulet::amulets.end() : data::Amulet::amulets.find(worn->second.name);

	for (set<unsigned char>::const_iterator k = keys.begin(); k != keys.end(); ++k) {
		map<unsigned char, Item>::iterator i = Inventory::items().find(*k);
		if (i == Inventory::items().end())
			return; // huh? this can't happen
		map<string, data::Amulet*>::iterator a = data::Amulet::amulets.find(i->second.name);
		if (a == data::Amulet::amulets.end())
			return; // this is no amulet

		/* TODO: is this amulet better than the one we wear? */
		/* TODO: if we find a better amulet, take off current amulet and return */
	}

	if (best_key == '\0')
		return; // no new amulet to put on or wearing best amulet

	/* put on this amulet */
	World::setAction(static_cast<action::Action*> (new action::PutOn(this, best_key, PRIORITY_AMULET_WEAR)));
}
