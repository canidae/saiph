#include "Analyzers/Excalibur.h"

#include "Inventory.h"
#include "Saiph.h"
#include "World.h"
#include "EventBus.h"
#include "Actions/Dip.h"
#include "Actions/Move.h"
#include "Events/ChangedInventoryItems.h"
#include "Events/ReceivedItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Excalibur::Excalibur() : Analyzer("Excalibur"), _long_sword_key(ILLEGAL_ITEM) {
	EventBus::registerEvent(ChangedInventoryItems::ID, this);
	EventBus::registerEvent(ReceivedItems::ID, this);
}

/* methods */
void Excalibur::analyze() {
	if (Saiph::experience() < 8)
		return;
	else if (Saiph::alignment() != LAWFUL)
		return;
	else if (Saiph::blind() || Saiph::confused() || Saiph::stunned() || Saiph::hallucinating())
		return; // don't move when blind/confused/stunned/hallucinating
	else if (World::level().branch() == BRANCH_MINES || World::level().branch() == BRANCH_MINETOWN)
		return; // don't ever dip in minetown...
	/* do we have a long sword? */
	unsigned char got_long_sword = ILLEGAL_ITEM;
	for (map<unsigned char, Item>::iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i) {
		if (i->second.name() == EXCALIBUR_LONG_SWORD) {
			got_long_sword = i->first;
			break;
		}
	}
	if (got_long_sword == ILLEGAL_ITEM)
		return;
	/* path to nearest fountain */
	Tile tile = World::shortestPath(FOUNTAIN);
	if (tile.cost() >= UNPASSABLE)
		return; // can't get to any fountains
	if (tile.direction() == NOWHERE) {
		/* standing on (in?) fountain, dip */
		World::setAction(static_cast<action::Action*> (new action::Dip(this, got_long_sword, ILLEGAL_ITEM, PRIORITY_EXCALIBUR_DIP)));
	} else {
		/* move towards fountain */
		World::setAction(static_cast<action::Action*> (new action::Move(this, tile, PRIORITY_EXCALIBUR_DIP)));
	}
}

void Excalibur::parseMessages(const string& messages) {
	if (messages.find(MESSAGE_RECEIVED_EXCALIBUR, 0) != string::npos) {
		/* alright! disable this analyzer */
		World::unregisterAnalyzer(this);
	}
}

void Excalibur::onEvent(event::Event* const event) {
	if (event->id() == ChangedInventoryItems::ID) {
		ChangedInventoryItems* e = static_cast<ChangedInventoryItems*> (event);
		for (set<unsigned char>::iterator k = e->keys().begin(); k != e->keys().end(); ++k) {
			map<unsigned char, Item>::iterator i = Inventory::items().find(*k);
			if (*k == _long_sword_key && i == Inventory::items().end()) {
				_long_sword_key = ILLEGAL_ITEM;
			} else if (i != Inventory::items().end() && i->second.name() == EXCALIBUR_NAME) {
				World::unregisterAnalyzer(this); 
				break; // we already have Excalibur
			} else if (i != Inventory::items().end() && i->second.name() == EXCALIBUR_LONG_SWORD) {
				_long_sword_key = *k;
			}
		}
	} else if (event->id() == ReceivedItems::ID) {
		if (_long_sword_key == ILLEGAL_ITEM) {
			ReceivedItems* e = static_cast<ReceivedItems*> (event);
			for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
				if (i->second.name() != EXCALIBUR_LONG_SWORD)
					continue;
				_long_sword_key = i->first;
				break;
			}
		}
	}
}
