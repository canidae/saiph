#include "Analyzers/DiggingTool.h"

#include <stdlib.h>
#include "Debug.h"
#include "EventBus.h"
#include "Inventory.h"
#include "Item.h"
#include "Saiph.h"
#include "World.h"
#include "Data/Pickaxe.h"
#include "Events/Event.h"
#include "Events/Beatify.h"
#include "Events/ChangedInventoryItems.h"
#include "Events/ReceivedItems.h"
#include "Events/WantItems.h"
#include "Events/GotDiggingTool.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
DiggingTool::DiggingTool() : Analyzer("DiggingTool"), _digging_tool_key(0) {
	/* register events */
	EventBus::registerEvent(WantItems::ID, this);
	EventBus::registerEvent(ChangedInventoryItems::ID, this);
	EventBus::registerEvent(ReceivedItems::ID, this);
}

/* methods */
void DiggingTool::onEvent(Event * const event) {
	if (event->id() == ChangedInventoryItems::ID || event->id() == ReceivedItems::ID) {
		findDigger();
	} else if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
		int key;
		int score;

		if (e->dropping()) rankDiggers(key, score, NULL, &e->items());
		else rankDiggers(key, score, &Inventory::items(), &e->items());

		if (key >= 0) {
			Item& item = e->items()[(unsigned char)key];
			item.want(item.count());
		}
	}
}

/* private methods */
int DiggingTool::scoreDigger(const Item& item) {
	if (data::Pickaxe::pickaxes().find(item.name()) == data::Pickaxe::pickaxes().end() || item.beatitude() == CURSED)
		return -1;
	int score = 1;
	if (item.beatitude() != BEATITUDE_UNKNOWN) score += 2;
	// prefer pick-axes - they're lighter
	if (item.name() == "pick-axe") score++;
	Debug::custom(name()) << "Scoring " << item.name() << " at " << score << endl;
	return score;
}

// key has to default -1 because the maps from actions::Loot use 0.. as key range
void DiggingTool::rankDiggers(int& key, int& score, const map<unsigned char, Item> *fixed, const map<unsigned char, Item> *variable) {
	key = -1;
	score = -1;

	if (fixed) {
		for (map<unsigned char, Item>::const_iterator i = fixed->begin(); i != fixed->end(); ++i)
			score = max(score, scoreDigger(i->second));
	}

	for (map<unsigned char, Item>::const_iterator i = variable->begin(); i != variable->end(); ++i) {
		int nscore = scoreDigger(i->second);
		if (nscore > score) {
			score = nscore;
			key = i->first;
		}
	}

	Debug::custom(name()) << "Best digger is " << key << " with score " << score << endl;
}

void DiggingTool::findDigger() {
	int key;
	int score;
	rankDiggers(key, score, NULL, &Inventory::items());

	if (key >= 0 && Inventory::itemAtKey(key).beatitude() == BEATITUDE_UNKNOWN) {
		Beatify b((unsigned char)key, 175);
		EventBus::broadcast(&b);
		key = -1; // not usable yet
	}

	if (_digging_tool_key != key) {
		_digging_tool_key = key;
		GotDiggingTool g(key >= 0 ? (unsigned char)key : ILLEGAL_ITEM);
		EventBus::broadcast(&g);
	}
}
