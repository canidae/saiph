#include "Analyzers/Amulet.h"

#include <stdlib.h>
#include "Debug.h"
#include "EventBus.h"
#include "Globals.h"
#include "Inventory.h"
#include "Item.h"
#include "World.h"
#include "Actions/Call.h"
#include "Actions/PutOn.h"
#include "Actions/Remove.h"
#include "Data/Amulet.h"
#include "Events/Beatify.h"
#include "Events/Event.h"
#include "Events/ChangedInventoryItems.h"
#include "Events/ReceivedItems.h"
#include "Events/WantItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Amulet::Amulet() : Analyzer("Amulet"), _amulet_key(ILLEGAL_ITEM) {
	/* register events */
	EventBus::registerEvent(WantItems::ID, this);
}

void Amulet::analyze() {
	if (!Saiph::polymorphed()) {
		char best_amulet = bestAmulet();
		char worn_amulet = Inventory::keyForSlot(SLOT_AMULET);
		//Debug::custom("Amulet") << "best=" << best_amulet << ", cur=" << worn_amulet << endl;
		if (worn_amulet != best_amulet) {
			if (worn_amulet == ILLEGAL_ITEM) {
				//Debug::custom("Amulet") << "Will wear best." << endl;
				World::setAction(new action::PutOn(this, best_amulet, PRIORITY_AMULET_WEAR));
			} else if (Inventory::itemInSlot(worn_amulet).beatitude() != CURSED) {
				bool harm = amuletScore(false, worn_amulet, Inventory::itemInSlot(worn_amulet)) < 0;
				//Debug::custom("Amulet") << "To remove " << (harm ? "harmful " : "superceded ") << "amulet." << endl;
				World::setAction(new action::Remove(this, worn_amulet, harm ? PRIORITY_AMULET_REMOVE_HARM : PRIORITY_AMULET_WEAR));
			}
		}
	}
}

void Amulet::parseMessages(const string& messages) {
	if (messages.find(STRANGULATION_5TTL) != string::npos) {
		Inventory::update();
	} else if (messages.find(RESTFUL_SLEEP) != string::npos) {
		char worn_amulet = Inventory::keyForSlot(SLOT_AMULET);
		World::queueAction(new action::Call(this, worn_amulet, "amulet of restful sleep"));
	}
}

/* methods */
void Amulet::onEvent(Event* const event) {
	if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (wantItem(i->second))
				i->second.want(i->second.count());
		}
	}
}

/* private methods */
bool Amulet::wantItem(const Item& item) {
	if (item.name().find("strangulation") != string::npos || item.name().find("restful") != string::npos)
		return false;
	if (item.beatitude() == CURSED)
		return false;
	return data::Amulet::amulets().find(item.name()) != data::Amulet::amulets().end();
}

int Amulet::amuletScore(bool reflection, char key, const Item& i) {
	map<const string, const data::Amulet*>::const_iterator a = data::Amulet::amulets().find(i.name());
	if (a == data::Amulet::amulets().end())
		return -1;
	if (i.beatitude() == CURSED)
		return -1;
	if (i.beatitude() == BEATITUDE_UNKNOWN) {
		Beatify b(key, 200);
		EventBus::broadcast(&b);
		return -1;
	}
	if ((i.name().find("restful") != string::npos) || i.name().find("strangulation") != string::npos)
		return -1;
	// priorities: reflection > life saving > unknown > unchanging > ESP
	if (!reflection && i.name().find("reflection") != string::npos)
		return 5; // amulet of reflection and no reflection from armor
	if (i.name().find("life saving") != string::npos)
		return 4; // life saving amulet is best
	else if (i.name().find("unchanging") != string::npos)
		return 2;
	else if (i.name().find("ESP") != string::npos)
		return 1;

	return 3; // unknown, assumed to be possible life saving
}

unsigned char Amulet::bestAmulet() {
	Item suit = Inventory::itemInSlot(SLOT_SUIT);
	Item shield = Inventory::itemInSlot(SLOT_SHIELD);
	bool reflection = suit.name().find("silver dragon") != string::npos || shield.name().find("silver shield") != string::npos || shield.name().find("reflection") != string::npos;

	unsigned char best = ILLEGAL_ITEM;
	int best_score     = 0;

	for (map<unsigned char, Item>::const_iterator i = Inventory::items().begin(); i != Inventory::items().end(); ++i) {
		int score = amuletScore(reflection, i->first, i->second);
		//Debug::custom("Amulet") << "Amulet " << i->second.name() << " score, " << score << endl;
		if (score > best_score) {
			best_score = score;
			best = i->first;
		}
	}
	//Debug::custom("Amulet") << "Best is " << best << endl;
	return best;
}
