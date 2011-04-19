#include "Analyzers/Wish.h"

#include <string>
#include "Actions/Answer.h"
#include "Actions/Call.h"
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
Wish::Wish() : Analyzer("Wish"), wand_of_wishing_key(ILLEGAL_ITEM), charging_key(ILLEGAL_ITEM), wished_for_charging(false), zapping_wand(false), extrinsics(0){
		EventBus::registerEvent(ReceivedItems::ID, this);
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
			extrinsics = currentExtrinsics();
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
				extrinsics = currentExtrinsics();
				string command = "blessed greased fixed +3 " + selectWish();
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
		if (Inventory::itemAtKey(wand_of_wishing_key).name().find("of wishing named EMPTY") != string::npos) {
			//if (charging_key != ILLEGAL_ITEM)
				// TODO: read charging, charge wand
			//else
				// TODO: wrest
		} else {
			currentExtrinsics();
			if (!haveReflection || !haveMR || !wearing("speed boots") || !wearing("gauntlets of power")) {
				zapping_wand = true;
			}
		}
	}
}

void Wish::onEvent(event::Event * const event) {
	if (event->id() == ReceivedItems::ID) {
		if (!wished_for_charging)
			return; // we didn't wish for charging, so who cares
		ReceivedItems* e = static_cast<ReceivedItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			charging_key = i->first;
			World::queueAction(static_cast<action::Action*> (new action::Call(this, charging_key, "scroll of charging")));
			break;
		}
		wished_for_charging = false;
	} else if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (i->second.name().find("of charging") != string::npos)
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
	} else if (!wearing("speed boots"))
		return "speed boots";
	else if (!wearing("gauntlets of power"))
		return "gauntlets of power";
	//just some survival items
	//TODO: wait until we wear amulets
//	if (!reflectionAmulet)
//		return "amulet of life saving";
	return "potion of full healing";
}

bool Wish::wearing(const string& name) {
	for (map<unsigned char, Item>::iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i)
		if (i->second.name().find(name) != string::npos)
			return true;
	return false;
}

unsigned long long Wish::currentExtrinsics() {
	/* TODO: look at other sources of MR, artifacts, etc */
	unsigned long long extrinsics = 0;
	haveReflection = false;
	haveMR = false;
	reflectionArmor = false;
	MRarmor = false;
	MRcloak = false;
	reflectionAmulet = false;
	reflectionShield = false;
	if (wearing("silver dragon scale mail") || wearing("silver dragon scales")) {
		haveReflection = true;
		reflectionArmor = true;
	}
	if (wearing("amulet of reflection")) {
		haveReflection = true;
		reflectionAmulet = true;
	}
	if (wearing("shield of reflection")) {
		haveReflection = true;
		reflectionAmulet = true;
	}
	if (wearing("gray dragon scale mail") || wearing("gray dragon scales")) {
		haveMR = true;
		MRarmor = true;
	}
	if (wearing("cloak of magic resistance")) {
		haveMR = true;
		MRcloak = false;
	}
	if (haveMR)
		extrinsics |= PROPERTY_MAGICRES;
	if (haveReflection)
		extrinsics |= PROPERTY_REFLECTION;
	return extrinsics;
}
	
