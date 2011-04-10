#include "Analyzers/Wand.h"

#include "Debug.h"
#include "EventBus.h"
#include "Globals.h"
#include "Inventory.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Call.h"
#include "Actions/Engrave.h"
#include "Data/Item.h"
#include "Data/Wand.h"
#include "Events/Beatify.h"
#include "Events/ReceivedItems.h"
#include "Events/WantItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Wand::Wand() : Analyzer("Wand"), _engrave_test_wand_key(0) {
	/* register events */
	EventBus::registerEvent(ReceivedItems::ID, this);
	EventBus::registerEvent(WantItems::ID, this);
}

/* methods */
void Wand::analyze() {
	if (Inventory::itemAtKey(_engrave_test_wand_key) == Inventory::NO_ITEM)
		return;
	if (!action::Engrave::canEngrave())
		return;
	World::setAction(static_cast<action::Action*> (new action::Engrave(this, ELBERETH "\n", _engrave_test_wand_key, PRIORITY_WAND_ENGRAVE_TEST)));
}

void Wand::onEvent(Event * const event) {
	if (event->id() == ReceivedItems::ID) {
		ReceivedItems* e = static_cast<ReceivedItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			map<const string, const data::Wand*>::const_iterator w = data::Wand::wands().find(i->second.name());
			if (w == data::Wand::wands().end())
				continue; // not a wand
			if (i->second.beatitude() == BEATITUDE_UNKNOWN) {
				// beatify, but it's not that important
				Beatify b(i->first, 10);
				EventBus::broadcast(&b);
			}
			if (w->second->properties() & PROPERTY_RANDOM_APPEARANCE)
				_engrave_test_wand_key = i->first;
		}
	} else if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			map<const string, const data::Wand*>::const_iterator w = data::Wand::wands().find(i->second.name());
			if (w == data::Wand::wands().end())
				continue; // not a wand
			i->second.want(i->second.count());
		}
	}
}

void Wand::actionCompleted(const std::string& messages) {
	if (_engrave_test_wand_key != 0 && World::lastActionID() == action::Engrave::ID) {
		// name wand if we can identify it
		/* TODO: now she'll just discard all wands she can't identify directly, need a better handling */
		for (map<const string, const data::Wand*>::const_iterator w = data::Wand::wands().begin(); w != data::Wand::wands().end(); ++w) {
			if (w->second->engraveMessage() == "")
				continue;
			if (messages.find(w->second->engraveMessage()) != string::npos) {
				if (w->second->name() == "wand of death" || w->second->name() == "wand of sleep")
					World::queueAction(static_cast<action::Action*> (new action::Call(this, _engrave_test_wand_key, "wand of death/sleep")));
				else if (w->first == "wand of teleportation" || w->first == "wand of cancellation" || w->first == "wand of make invisible")
					World::queueAction(static_cast<action::Action*> (new action::Call(this, _engrave_test_wand_key, "wand of vanish")));
				else
					World::queueAction(static_cast<action::Action*> (new action::Call(this, _engrave_test_wand_key, w->first)));
				_engrave_test_wand_key = 0;
				return;
			}
		}
		World::queueAction(static_cast<action::Action*> (new action::Call(this, _engrave_test_wand_key, "wand of unknown")));
		_engrave_test_wand_key = 0;
	}
}