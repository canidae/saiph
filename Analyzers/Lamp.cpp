#include "Lamp.h"

#include "../EventBus.h"
#include "../Inventory.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Actions/Action.h"
#include "../Actions/Apply.h"
#include "../Actions/Call.h"
#include "../Actions/Name.h"
#include "../Data/Lamp.h"
#include "../Events/ChangedInventoryItems.h"
#include "../Events/WantItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Lamp::Lamp() : Analyzer("Lamp"), _lamp_key(ILLEGAL_ITEM), _seen_oil_lamp(false), _seen_magic_lamp(false) {
	EventBus::registerEvent(ChangedInventoryItems::ID, this);
	EventBus::registerEvent(WantItems::ID, this);
}

/* methods */
void Lamp::analyze() {
	if (_lamp_key == ILLEGAL_ITEM || Saiph::encumbrance() >= OVERTAXED)
		return; // no lamp/lantern or can't use it
	/* find lamp */
	findLamp();
	if (_lamp_key == ILLEGAL_ITEM)
		return; // no more lamps/lanterns in inventory
	/* turn lamp/lantern on */
	World::setAction(static_cast<action::Action*> (new action::Apply(this, _lamp_key, 100, false)));
}

void Lamp::parseMessages(const string& messages) {
	if (messages.find(LAMP_TURNED_ON) != string::npos) {
		/* lamp/lantern turned on */
		map<unsigned char, Item>::iterator l = Inventory::items().find(_lamp_key);
		if (l == Inventory::items().end())
			Inventory::updated(false); // something must've happened to our inventory
		else
			l->second.additional(LAMP_LIT); // set "additional" instead of flashing inventory
	}
	if (messages.find(LAMP_LAMP_GOES_OUT) != string::npos || messages.find(LAMP_LANTERN_GOES_OUT) != string::npos) {
		/* probably got engulfed or something, lamp/lantern went out */
		map<unsigned char, Item>::iterator i = Inventory::items().find(_lamp_key);
		if (i != Inventory::items().end())
			i->second.additional(""); // reset "additional" instead of flashing inventory
	} else if (messages.find(LAMP_LAMP_OUT) != string::npos || messages.find(LAMP_OIL_LAMP_OUT) != string::npos || messages.find(LAMP_LANTERN_OUT) != string::npos || messages.find(LAMP_LAMP_OUT_OF_POWER) != string::npos || messages.find(LAMP_NO_OIL) != string::npos) {
		/* our lamp/lantern got depleted.
		 * name it "oil lamp" if it was a "lamp" and discard it */
		map<unsigned char, Item>::iterator i = Inventory::items().find(_lamp_key);
		if (i != Inventory::items().end()) {
			i->second.additional(""); // reset "additional" instead of flashing inventory
			/* if the name is "lamp" then it's an oil lamp */
			if (i->second.name() == "lamp") {
				World::queueAction(static_cast<action::Action*> (new action::Call(this, i->first, "oil lamp")));
				i->second.name("oil lamp");
				_seen_oil_lamp = true;
			}
			/* since it's empty, we'll also discard it */
			World::queueAction(static_cast<action::Action*> (new action::Name(this, i->first, DISCARD)));
		}
	}
}

void Lamp::onEvent(event::Event * const event) {
	if (event->id() == ChangedInventoryItems::ID) {
		/* inventory changed, find lamp */
		findLamp();
	} else if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (data::Lamp::lamps().find(i->second.name()) != data::Lamp::lamps().end())
				i->second.want(i->second.count());
		}
	}
}

/* private methods */
void Lamp::findLamp() {
	map<unsigned char, Item>::iterator l = Inventory::items().find(_lamp_key);
	if (l != Inventory::items().end() && (data::Lamp::lamps().find(l->second.name()) != data::Lamp::lamps().end()))
		return; // _lamp_key already is a lamp
	for (l = Inventory::items().begin(); l != Inventory::items().end(); ++l) {
		if (data::Lamp::lamps().find(l->second.name()) == data::Lamp::lamps().end())
			continue; // not a lamp
		/* this should be a lamp/lantern */
		_lamp_key = l->first;
		/* maybe we know what kind of lamp it is? */
		if (_seen_oil_lamp && !_seen_magic_lamp && l->second.name() == "lamp") {
			/* this must be a magic lamp */
			World::queueAction(static_cast<action::Action*> (new action::Call(this, l->first, "magic lamp")));
			_seen_magic_lamp = true;
		} else if (!_seen_oil_lamp && _seen_magic_lamp && l->second.name() == "lamp") {
			/* this must be an oil lamp */
			World::queueAction(static_cast<action::Action*> (new action::Call(this, l->first, "oil lamp")));
			_seen_oil_lamp = true;
		}
		return;
	}
	/* no lamp/lantern */
	_lamp_key = ILLEGAL_ITEM;
}
