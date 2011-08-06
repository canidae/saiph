#include "Analyzers/Wish.h"

#include <string>
#include "Actions/Answer.h"
#include "Actions/Call.h"
#include "Actions/Charge.h"
#include "Actions/Name.h"
#include "Actions/Wish.h"
#include "EventBus.h"
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
Wish::Wish() : Analyzer("Wish"), wand_of_wishing_key(ILLEGAL_ITEM), charging_key(ILLEGAL_ITEM), named_empty(false), named_full(false), wished_for_charging(false), wished_for_aor(false), wished_for_speed(false), wished_for_gop(false), will_wish(false) {
	EventBus::registerEvent(ReceivedItems::ID, this);
	EventBus::registerEvent(WantItems::ID, this);
}

/* methods */
void Wish::parseMessages(const string& messages) {
	if (messages.find(MESSAGE_FOR_WHAT_DO_YOU_WISH) != string::npos) {
		for (map<unsigned char, Item>::iterator l = Inventory::items().begin(); l != Inventory::items().end(); ++l) {
			Debug::custom(name()) << "item name: " << l->second.name() << endl;
			if (l->second.name().find("wand of wishing") != string::npos)
				wand_of_wishing_key = l->first;
		}
		if (wand_of_wishing_key == ILLEGAL_ITEM) {
			string command = "blessed greased fixed +3 " + selectWish();
			Debug::custom(name()) << "Wishing for " << command << endl;
			command.append("\n");
			World::queueAction(static_cast<action::Action*> (new action::Answer(this, command)));
			return;
		} else {
			if (charging_key == ILLEGAL_ITEM) {
				string command = "3 blessed scrolls of charging\n";
				Debug::custom(name()) << "Wishing for charging" << endl;
				World::queueAction(static_cast<action::Action*> (new action::Answer(this, command)));
				wished_for_charging = true;
			} else {
				string command = "blessed greased fixed +3 " + selectWish();
				// identify that we need to type-name these things.
				if (command.find("amulet of reflection") != string::npos)
					wished_for_aor = true;
				else if (command.find("speed boots") != string::npos)
					wished_for_speed = true;
				else if (command.find("gauntlets of power") != string::npos)
					wished_for_gop = true;
				Debug::custom(name()) << "Wishing for " << command << endl;
				command.append("\n");
				World::queueAction(static_cast<action::Action*> (new action::Answer(this, command)));
			}
		}
	}
}

void Wish::analyze() {
	//if we have a wand of wishing, we need to decide if we should zap
	if (wand_of_wishing_key != ILLEGAL_ITEM) {
		if (Inventory::itemAtKey(wand_of_wishing_key).name().find("named EMPTY") != string::npos) {
			/* this is really counter-intutitive, but we need it
			 * since we have to find out it's empty in the middle
			 * of an action, and then we need to formally name it.
			 */
			if (!named_empty) {
				World::queueAction(static_cast<action::Action*> (new action::Name(this, wand_of_wishing_key, "EMPTY")));
				named_empty = true;
			}
			will_wish = false;
			if (charging_key != ILLEGAL_ITEM && Inventory::itemAtKey(charging_key).beatitude() == BLESSED && Inventory::itemAtKey(wand_of_wishing_key).name().find("full") == string::npos) {
				if (!haveReflection || !haveMR || !wearing("speed boots") || !wearing("gauntlets of power")) {
					World::setAction(static_cast<action::Action*> (new action::Charge(this, charging_key, wand_of_wishing_key, 100)));
					if (named_full == false) {
						World::queueAction(static_cast<action::Action*> (new action::Name(this, wand_of_wishing_key, "full")));
						named_full = true;
					}
					Inventory::update();
					will_wish = true;
				}
			}	
			// TODO: wrest
		} else {
			if (!haveReflection || !haveMR || !wearing("speed boots") || !wearing("gauntlets of power")) {
				// we don't use markers, this is all we want
				will_wish = true;
			}
		}
		if (will_wish && Inventory::itemAtKey(wand_of_wishing_key) != Inventory::NO_ITEM) {
			action::Wish* wishAction = new action::Wish(this, wand_of_wishing_key, 100);
			if (wishAction->canEngrave())
				World::setAction(static_cast<action::Action*> (wishAction));
			will_wish = false;
		}	
	}
}

void Wish::onEvent(event::Event* const event) {
	if (event->id() == ReceivedItems::ID) {
		if (!wished_for_charging && !wished_for_aor)
			return; // we didn't wish for something that we want to name
		ReceivedItems* e = static_cast<ReceivedItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (wished_for_charging) {
				charging_key = i->first;
				World::queueAction(static_cast<action::Action*> (new action::Call(this, charging_key, "scroll of charging")));
			} else if (wished_for_aor) {
				World::queueAction(static_cast<action::Action*> (new action::Call(this, i->first, "amulet of reflection")));
			} else if (wished_for_speed) {
				World::queueAction(static_cast<action::Action*> (new action::Call(this, i->first, "speed boots")));
			} else if (wished_for_gop) {
				World::queueAction(static_cast<action::Action*> (new action::Call(this, i->first, "gauntlets of power")));
			}
			break;
		}
		wished_for_charging = wished_for_aor = wished_for_speed = wished_for_gop = false;
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
}

string Wish::selectWish() {
	//if (wish_from_wand && !have_charging)
	//	return "3 scrolls of charging";
	if (!haveMR) {
		if (reflectionArmor)
			return "cloak of magic resistance";
		return "gray dragon scale mail";
	} else if (!haveReflection) {
		if (MRarmor)
			return "amulet of reflection";
		return "silver dragon scale mail";
	} else if (!wearing("speed boots")) {
		return "speed boots";
	} else if (!wearing("gauntlets of power")) {
		return "gauntlets of power";
	}
	//just some survival items
	//TODO: wait until we wear amulets
	//	if (!reflectionAmulet)
	//		return "amulet of life saving";
	return "potion of full healing";
}

bool Wish::wearing(const string& name) {
	for (map<unsigned char, Item>::iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i) {
		if (i->second.name().find(name) != string::npos)
			return true;
	}
	return false;
}
