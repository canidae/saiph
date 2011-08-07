#include "Analyzers/Wish.h"

#include <string>
#include "Actions/Answer.h"
#include "Actions/Call.h"
#include "Actions/Charge.h"
#include "Actions/Engrave.h"
#include "Actions/Name.h"
#include "Actions/Wish.h"
#include "EventBus.h"
#include "Events/ChangedInventoryItems.h"
#include "Events/ReceivedItems.h"
#include "Events/WantItems.h"
#include "Inventory.h"
#include "Item.h"
#include "Globals.h"
#include "Saiph.h"
#include "World.h"
#include "Debug.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Wish::Wish() : Analyzer("Wish"), wand_of_wishing_key(ILLEGAL_ITEM), charging_key(ILLEGAL_ITEM), named_empty(false), named_full(false), will_wish(false) {
	EventBus::registerEvent(ReceivedItems::ID, this);
	EventBus::registerEvent(ChangedInventoryItems::ID, this);
	EventBus::registerEvent(WantItems::ID, this);
}

/* methods */
void Wish::parseMessages(const string& messages) {
	if (messages.find(MESSAGE_FOR_WHAT_DO_YOU_WISH) != string::npos) {
		bool from_wand = (World::lastActionID() == action::Engrave::ID || World::lastActionID() == action::Wish::ID); // or zap
		string command = "blessed greased fixed +3 " + selectWish(from_wand);
		Debug::custom(name()) << "Wishing for " << command << endl;
		command.append("\n");
		World::queueAction(new action::Answer(this, command));
	}
}

void Wish::analyze() {
	//if we have a wand of wishing, we need to decide if we should zap
	//TODO: for now always drain wands
	if (wand_of_wishing_key != ILLEGAL_ITEM) {
		if (Inventory::itemAtKey(wand_of_wishing_key).name().find("named EMPTY") != string::npos) {
			/* this is really counter-intutitive, but we need it
			 * since we have to find out it's empty in the middle
			 * of an action, and then we need to formally name it.
			 */
			if (!named_empty) {
				World::queueAction(new action::Name(this, wand_of_wishing_key, "EMPTY"));
				named_empty = true;
			}
			will_wish = false;
			if (charging_key != ILLEGAL_ITEM && Inventory::itemAtKey(charging_key).beatitude() == BLESSED && Inventory::itemAtKey(wand_of_wishing_key).name().find("full") == string::npos) {
				World::setAction(static_cast<action::Action*> (new action::Charge(this, charging_key, wand_of_wishing_key, 100)));
				if (named_full == false) {
					World::queueAction(new action::Name(this, wand_of_wishing_key, "full"));
					named_full = true;
				}
				Inventory::update();
				will_wish = true;
			}	
			// TODO: wrest
		} else {
			will_wish = true;
		}
		if (will_wish && Inventory::itemAtKey(wand_of_wishing_key) != Inventory::NO_ITEM) {
			if (action::Engrave::canEngrave())
				World::setAction(new action::Wish(this, wand_of_wishing_key, 100));
			will_wish = false;
		}
	}
}

void Wish::onEvent(event::Event* const event) {
	if (event->id() == ReceivedItems::ID) {
		ReceivedItems* e = static_cast<ReceivedItems*> (event);
		if (World::lastAction()->id() == action::Answer::ID && World::lastAction()->analyzer() == this && e->items().size() > 0 && _last_wish_call.size() > 0) {
			// XXX it's possible we'll get an extra item in the same turn in some odd cases (mail is the most obvious)
			// hopefully it'll come in at the lowest character since it was first
			unsigned char oldest_new_key = e->items().begin()->first;
			World::queueAction(new action::Call(this, oldest_new_key, _last_wish_call));
		}
	} else if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (i->second.name().find("charging") != string::npos)
				i->second.want(i->second.count());
			else if (i->second.name().find("of wishing") != string::npos)
				i->second.want(i->second.count());
			else if (i->second.name().find("full") != string::npos)
				i->second.want(i->second.count());
		}
	}

	if (event->id() == ReceivedItems::ID || event->id() == ChangedInventoryItems::ID) {
		wand_of_wishing_key = charging_key = ILLEGAL_ITEM;
		for (map<unsigned char, Item>::const_iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i) {
			if (i->second.name().find("wand of wishing") != string::npos)
				wand_of_wishing_key = i->first;
			if (i->second.name().find("scroll of charging") != string::npos)
				charging_key = i->first;
		}
	}
}

string Wish::selectWish(bool wish_from_wand) {
	_last_wish_call.clear();
	if (wish_from_wand && charging_key == ILLEGAL_ITEM) {
		_last_wish_call = "scroll of charging";
		return "3 scrolls of charging";
	}
	bool have_mr = (have("cloak of magic resistance") || have("gray dragon scale") || have("Magicbane"));
	bool reflect = (have("amulet of reflection") || have("shield of reflection") || have("silver shield") || have("silver dragon scale"));

	if (wish_from_wand && !have_mr && !reflect)
		return "silver dragon scale mail";

	if (!have_mr) {
		if (have("silver dragon scale"))
			return _last_wish_call = "cloak of magic resistance";
		return "gray dragon scale mail";
	} else if (!reflect) {
		if (have("gray dragon scale"))
			return _last_wish_call = "amulet of reflection";
		return "silver dragon scale mail";
	} else if (!have("speed boots")) {
		return _last_wish_call = "speed boots";
	} else if (!have("gauntlets of power")) {
		return _last_wish_call = "gauntlets of power";
	}
	//just some survival items
	if (!have("amulet of reflection"))
		return _last_wish_call = "amulet of life saving";
	_last_wish_call = "potion of full healing";
	return "3 potions of full healing";
}

bool Wish::have(const string& name) {
	for (map<unsigned char, Item>::iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i) {
		if (i->second.name().find(name) != string::npos)
			return true;
	}
	return false;
}
