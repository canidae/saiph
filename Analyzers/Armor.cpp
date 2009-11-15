#include "Armor.h"

#include "../EventBus.h"
#include "../Inventory.h"
#include "../Item.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Actions/Answer.h"
#include "../Data/Armor.h"
#include "../Events/Beatify.h"
#include "../Events/Event.h"
#include "../Events/ChangedInventoryItems.h"
#include "../Events/ReceivedItems.h"
#include "../Events/WantItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Armor::Armor() : Analyzer("Armor"), _change_armor(false) {
	/* register events */
	EventBus::registerEvent(ChangedInventoryItems::ID, this);
	EventBus::registerEvent(ReceivedItems::ID, this);
	EventBus::registerEvent(WantItems::ID, this);
}

/* methods */
void Armor::parseMessages(const string& messages) {
	string::size_type pos;
	if (World::question() && (pos = messages.find(MESSAGE_FOOCUBUS_QUESTION)) != string::npos) {
		/* A foocubus wants to remove something. Check the beatitude of that armor (and any armor covering it) */
		bool remove = false;
		if (messages.find(" cloak, ", pos) == pos || messages.find(" wrapping, ", pos) == pos || messages.find(" robe, ", pos) == pos || messages.find(" apron, ", pos) == pos || messages.find(" smock, ", pos) == pos) {
			if (Inventory::itemInSlot(SLOT_CLOAK).beatitude() == CURSED || Inventory::itemInSlot(SLOT_SUIT).beatitude() == CURSED || Inventory::itemInSlot(SLOT_SHIRT).beatitude() == CURSED)
				remove = true;
		} else if (messages.find(" suit, ", pos) == pos) {
			if (Inventory::itemInSlot(SLOT_SUIT).beatitude() == CURSED || Inventory::itemInSlot(SLOT_SHIRT).beatitude() == CURSED)
				remove = true;
		} else if (messages.find(" boots, ", pos) == pos) {
			if (Inventory::itemInSlot(SLOT_BOOTS).beatitude() == CURSED)
				remove = true;
		} else if (messages.find(" gloves, ", pos) == pos) {
			if (Inventory::itemInSlot(SLOT_GLOVES).beatitude() == CURSED)
				remove = true;
		} else if (messages.find(" shield, ", pos) == pos) {
			if (Inventory::itemInSlot(SLOT_SHIELD).beatitude() == CURSED)
				remove = true;
		} else if (messages.find(" helmet, ", pos) == pos) {
			if (Inventory::itemInSlot(SLOT_HELMET).beatitude() == CURSED)
				remove = true;
		} else if (messages.find(" shirt, ", pos) == pos) {
			if (Inventory::itemInSlot(SLOT_SHIRT).beatitude() == CURSED)
				remove = true;
		}
		World::setAction(static_cast<action::Action*> (new action::Answer(this, (remove ? YES : NO))));
	} else if (_change_armor) {
		//wearArmor();
	}
}

void Armor::onEvent(event::Event * const event) {
	if (event->id() == ChangedInventoryItems::ID) {
		ChangedInventoryItems* e = static_cast<ChangedInventoryItems*> (event);
		// TODO: check if we should change armor
	} else if (event->id() == ReceivedItems::ID) {
		ReceivedItems* e = static_cast<ReceivedItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (i->second.beatitude() != BEATITUDE_UNKNOWN || data::Armor::armors().find(i->second.name()) == data::Armor::armors().end())
				continue; // known beatitude or not armor
			Beatify b(i->first, 100);
			EventBus::broadcast(&b);
		}
		// TODO: check if we should change armor
	} else if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (wantItem(i->second))
				i->second.want(i->second.count());
		}
	}
}

/* private methods */
bool Armor::wantItem(const Item& item) {
	return Saiph::encumbrance() < BURDENED && item.beatitude() != CURSED && data::Armor::armors().find(item.name()) != data::Armor::armors().end();
}