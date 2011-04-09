#include "Analyzers/Wand.h"

#include "Debug.h"
#include "EventBus.h"
#include "Globals.h"
#include "Inventory.h"
#include "Saiph.h"
#include "World.h"
#include "Events/Beatify.h"
#include "Events/Event.h"
#include "Events/ReceivedItems.h"
#include "Events/WantItems.h"
#include "Data/Item.h"
#include "Actions/Engrave.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Wand::Wand() : Analyzer("Wand"), _engrave_test_wand_key(0) {
}

/* methods */
void Wand::parseMessages(const string& messages) {
	if (messages.find(WAND_WORN_OUT_MESSAGE, 0) != string::npos)
		_state = WAND_STATE_WANT_DIRTY_INVENTORY;
	if (_state == WAND_STATE_INIT) {
		if (saiph->inventory_changed) {
			if (_engrave_test_wand_key != 0 && !isUnidentifiedWand(_engrave_test_wand_key))
				_engrave_test_wand_key = 0;
			if (_engrave_test_wand_key == 0)
				findUnidentifiedWands();
		}
		if (saiph->getDungeonSymbol() != FLOOR || saiph->world->player.levitating || saiph->world->player.blind)
			return;
		if (_engrave_test_wand_key != 0)
			_state = WAND_STATE_DUST_X;
	} //no else here.  deliberate.
	if (_state == WAND_STATE_DUST_X) {
		if (messages.find(MESSAGE_ENGRAVE_ADD, 0) != string::npos) {
			command = YES;
			priority = PRIORITY_CONTINUE_ACTION;
		} else if (messages.find(MESSAGE_ENGRAVE_WITH, 0) != string::npos) {
			command = HANDS;
			priority = PRIORITY_CONTINUE_ACTION;
		} else if (messages.find(MESSAGE_ENGRAVE_DUST, 0) != string::npos ||
				messages.find(MESSAGE_ENGRAVE_DUST_ADD, 0) != string::npos ||
				messages.find(MESSAGE_ENGRAVE_FROST, 0) != string::npos ||
				messages.find(MESSAGE_ENGRAVE_FROST_ADD, 0) != string::npos) {
			command = "x\n";
			priority = PRIORITY_CONTINUE_ACTION;
			_state = WAND_STATE_WANTS_LOOK;
		} else {
			/* don't engrave if we're now where we can't or shouldn't */
			if (saiph->getDungeonSymbol() != FLOOR || saiph->world->player.levitating || saiph->world->player.blind)
				return;
			command = ENGRAVE;
			priority = PRIORITY_WAND_ENGRAVE_ID;
		}
	} else if (_state == WAND_STATE_WANTS_LOOK) {
		//we consumed a move before this, ensure we didn't get interrupted
		if (_engrave_test_wand_key == 0 || !isUnidentifiedWand(_engrave_test_wand_key)) {
			_engrave_test_wand_key = 0;
			_state = WAND_STATE_INIT;
		}
		command = LOOK;
		priority = PRIORITY_LOOK;
		_state = WAND_STATE_CONFIRM_LOOK;
	} else if (_state == WAND_STATE_CONFIRM_LOOK) {
		if (messages.find(MESSAGE_TEXT_DUSTED, 0) != string::npos ||
			messages.find(MESSAGE_TEXT_FROSTED, 0) != string::npos)
			_state = WAND_STATE_ENGRAVING;
		else
			_state = WAND_STATE_INIT;
	} //no else here
	if (_state == WAND_STATE_ENGRAVING) {
		if (messages.find(MESSAGE_ENGRAVE_ADD, 0) != string::npos) {
			command = YES;
			priority = PRIORITY_CONTINUE_ACTION;
		} else if (messages.find(WAND_DIGGING_MESSAGE, 0) != string::npos ||
				messages.find(WAND_LIGHTNING_MESSAGE, 0) != string::npos ||
				messages.find(WAND_FIRE_MESSAGE, 0) != string::npos) {
			command = ESCAPE;
			priority = PRIORITY_CONTINUE_ACTION;
			req.request = REQUEST_DIRTY_INVENTORY;
			saiph->request(req);
			_engrave_test_wand_key = 0;
			_state = WAND_STATE_INIT;
		} else if (messages.find(WAND_POLYMORPH_MESSAGE, 0) != string::npos) {
			string name = WAND_POLYMORPH_NAME;
			for (vector<pair<string, string> >::size_type i = 0; i < _wand_engrave_messages.size(); i++)
				if (messages.find(_wand_engrave_messages[i].first, 0) != string::npos)
					name = _wand_engrave_messages[i].second;
			req.request = REQUEST_CALL_ITEM;
			req.data = name;
			req.key = _engrave_test_wand_key;
			saiph->request(req);
			_state = WAND_STATE_WANT_DIRTY_INVENTORY;
		} else if (messages.find(MESSAGE_ENGRAVE_DUST, 0) != string::npos ||
				messages.find(MESSAGE_ENGRAVE_DUST_ADD, 0) != string::npos ||
				messages.find(MESSAGE_ENGRAVE_FROST, 0) != string::npos ||
				messages.find(MESSAGE_ENGRAVE_FROST_ADD, 0) != string::npos) {
			command = "x\n";
			priority = PRIORITY_CONTINUE_ACTION;
			_state = WAND_STATE_READY_TO_NAME;
		} else if (messages.find(MESSAGE_ENGRAVE_WITH, 0) != string::npos) {
			command = _engrave_test_wand_key;
			priority = PRIORITY_CONTINUE_ACTION;
		} else {
			/* make sure we didn't get interrupted */
			if (_engrave_test_wand_key == 0 || !isUnidentifiedWand(_engrave_test_wand_key) || saiph->getDungeonSymbol() != FLOOR || saiph->world->player.levitating || saiph->world->player.blind) {
				_engrave_test_wand_key = 0;
				_state = WAND_STATE_INIT;
				return;
			}
			/* wands of wishing ask what we want to wish for */
			if (saiph->world->question)
				return;
			command = ENGRAVE;
			priority = PRIORITY_WAND_ENGRAVE_ID;
		}
	} else if (_state == WAND_STATE_READY_TO_NAME) {
		string name = WAND_NO_EFFECT_NAME;
		for (vector<pair<string, string> >::size_type i = 0; i < _wand_engrave_messages.size(); i++)
			if (messages.find(_wand_engrave_messages[i].first, 0) != string::npos)
				name = _wand_engrave_messages[i].second;
		req.request = REQUEST_CALL_ITEM;
		req.data = name;
		req.key = _engrave_test_wand_key;
		saiph->request(req);
		_state = WAND_STATE_WANT_DIRTY_INVENTORY;
	} else if (_state == WAND_STATE_WANT_DIRTY_INVENTORY) {
		req.request = REQUEST_DIRTY_INVENTORY;
		saiph->request(req);
		_state = WAND_STATE_INIT;
	}
}

void Wand::onEvent(Event * const event) {
	if (event->id() == ReceivedItems::ID) {
		ReceivedItems* e = static_cast<ReceivedItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			map<const string, const data::Wand*>::const_iterator w = data::Wand::wands().find(i->second.name());
			if (w == data::Wand::wands().end())
				continue; // not an wand
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
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i)
			i->second.want(i->second.count());
	}
}

void Wand::actionCompleted(const std::string& messages) {
	if (_engrave_test_wand_key != 0 && World::lastActionID() == action::Engrave::ID) {
		// addToMap in data::Wand if we could identify wand
	}
}