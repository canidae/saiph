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
Lamp::Lamp() : Analyzer("Lamp"), _lamp_key(0), _lamp_depleted(false), _seen_oil_lamp(false), _seen_magic_lamp(false) {
	EventBus::registerEvent(ChangedInventoryItems::ID, this);
	EventBus::registerEvent(WantItems::ID, this);
}

/* methods */
void Lamp::analyze() {
	if (_lamp_key == 0 || Saiph::encumbrance() >= OVERTAXED)
		return; // no lamp/lantern or got something more important to do
	/* find lamp */
	findLamp();
	if (_lamp_key == 0)
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
		Inventory::updated(false);
		_lamp_depleted = false;
	} else if (messages.find(LAMP_LAMP_OUT) != string::npos || messages.find(LAMP_OIL_LAMP_OUT) != string::npos || messages.find(LAMP_LANTERN_OUT) != string::npos || messages.find(LAMP_LAMP_OUT_OF_POWER) != string::npos || messages.find(LAMP_NO_OIL) != string::npos) {
		/* a lamp/lantern is depleted, discard it.
		 * problem is that we don't know which lamp/lantern it is
		 * as we may have multiple lamps/lanterns on.
		 * request inventory update, set a flag and remove the first
		 * unlit lamp we find */
		Inventory::updated(false);
		_lamp_depleted = true;
	}
}

void Lamp::onEvent(event::Event * const event) {
	if (event->id() == ChangedInventoryItems::ID) {
		ChangedInventoryItems* e = static_cast<ChangedInventoryItems*> (event);
		for (set<unsigned char>::iterator k = e->keys().begin(); k != e->keys().end(); ++k) {
			map<unsigned char, Item>::iterator i = Inventory::items().find(*k);
			if (i != Inventory::items().end() && data::Lamp::lamps().find(i->second.name()) != data::Lamp::lamps().end()) {
				if (_lamp_depleted && _lamp_key == 0 && i->second.additional() != LAMP_LIT) {
					/* a lamp got depleted and all our lamps were lit,
					 * any unlit lamps must be oil lamps */
					if (i->second.name() == "lamp" || i->second.name() == "magic lamp") {
						/* checking if name is "magic lamp" as well, in case we got amnesia */
						World::queueAction(static_cast<action::Action*> (new action::Call(this, i->first, "oil lamp")));
						i->second.name("oil lamp");
					}
					/* since it's empty, we'll also discard it */
					World::queueAction(static_cast<action::Action*> (new action::Name(this, i->first, DISCARD)));
				} else if (_seen_oil_lamp && i->second.name() == "lamp") {
					/* already seen oil lamps, this must be a magic lamp */
					World::queueAction(static_cast<action::Action*> (new action::Call(this, i->first, "magic lamp")));
					i->second.name("magic lamp");
				} else if (_seen_magic_lamp && i->second.name() == "lamp") {
					/* already seen magic lamps, this must be an oil lamp */
					World::queueAction(static_cast<action::Action*> (new action::Call(this, i->first, "oil lamp")));
					i->second.name("oil lamp");
				}
				if (!_seen_oil_lamp && i->second.name() == "oil lamp")
					_seen_oil_lamp = true;
				else if (!_seen_magic_lamp && i->second.name() == "magic lamp")
					_seen_magic_lamp = true;
			}
			_lamp_depleted = false;
		}
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
	if (l != Inventory::items().end() && l->second.additional() != LAMP_LIT && (data::Lamp::lamps().find(l->second.name()) != data::Lamp::lamps().end()))
		return; // _lamp_key already is an unlit lamp
	for (l = Inventory::items().begin(); l != Inventory::items().end(); ++l) {
		if (data::Lamp::lamps().find(l->second.name()) == data::Lamp::lamps().end())
			continue; // not a lamp
		else if (l->second.additional() == LAMP_LIT)
			continue; // already lit, we turn on every lamp/lantern so skip this
		/* this should be a lamp/lantern */
		_lamp_key = l->first;
		return;
	}
	/* no lamp/lantern */
	_lamp_key = 0;
}
