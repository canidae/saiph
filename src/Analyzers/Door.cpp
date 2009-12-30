#include "Analyzers/Door.h"

#include <stack>
#include "Debug.h"
#include "EventBus.h"
#include "Inventory.h"
#include "Item.h"
#include "Saiph.h"
#include "World.h"
#include "Actions/Answer.h"
#include "Actions/Kick.h"
#include "Actions/Move.h"
#include "Actions/Open.h"
#include "Actions/Unlock.h"
#include "Data/Key.h"
#include "Events/Event.h"
#include "Events/ChangedInventoryItems.h"
#include "Events/ReceivedItems.h"
#include "Events/WantItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Door::Door() : Analyzer("Door"), _unlock_tool_key(0) {
	/* register events */
	EventBus::registerEvent(ChangedInventoryItems::ID, this);
	EventBus::registerEvent(ReceivedItems::ID, this);
	EventBus::registerEvent(WantItems::ID, this);
}

/* methods */
void Door::analyze() {
	/* open closed doors */
	if (Saiph::blind() || Saiph::confused() || Saiph::stunned() || Saiph::hallucinating())
		return; // don't move to, or open doors when blind/confused/stunned/hallucinating

	/* go to nearest closed door and get it open somehow */
	unsigned int min_distance = UNREACHABLE;
	for (map<Point, int>::const_iterator d = World::level().symbols((unsigned char) CLOSED_DOOR).begin(); d != World::level().symbols((unsigned char) CLOSED_DOOR).end(); ++d) {
		Tile& tile = World::shortestPath(d->first);
		if (tile.cost() == UNREACHABLE)
			continue; // can't reach this door
		if (World::level().branch() == BRANCH_MINES && d->second == DOOR_LOCKED && (_unlock_tool_key == 0 || Inventory::items()[_unlock_tool_key].name() == "lock pick" || Inventory::items()[_unlock_tool_key].name() == "credit card"))
			continue; // don't kick/pick doors when we're in the mines
		if (d->second == DOOR_SHOP_INVENTORY && _unlock_tool_key == 0)
			continue; // shop and we got no means of opening it (well, except kicking)
		if (tile.distance() == 1 && !Saiph::inAPit()) {
			/* open/pick/kick door */
			if (d->second != DOOR_LOCKED) {
				World::setAction(static_cast<action::Action*> (new action::Open(this, tile.direction(), PRIORITY_DOOR_OPEN)));
			} else {
				/* we can't apply when we're overtaxed, but logically we can kick... */
				if (_unlock_tool_key == 0 || Saiph::encumbrance() >= OVERTAXED) {
					if (!Saiph::hurtLeg())
						World::setAction(static_cast<action::Action*> (new action::Kick(this, tile.direction(), PRIORITY_DOOR_OPEN)));
				} else {
					World::setAction(static_cast<action::Action*> (new action::Unlock(this, _unlock_tool_key, tile.direction(), PRIORITY_DOOR_OPEN)));
				}
			}
			_position = d->first;
			return;
		} else if (tile.distance() < min_distance) {
			/* go to door */
			World::setAction(static_cast<action::Action*> (new action::Move(this, tile, action::Move::calculatePriority(PRIORITY_DOOR_OPEN, tile.distance()))));
			min_distance = tile.distance();
		}
	}
}

void Door::parseMessages(const string& messages) {
	if (messages.find(MESSAGE_SUCCEED_UNLOCKING) != string::npos || messages.find(MESSAGE_SUCCEED_PICKING) != string::npos) {
		/* door unlocked */
		World::level().setDungeonSymbolValue(_position, UNKNOWN_SYMBOL_VALUE);
	} else if (messages.find(MESSAGE_DOOR_LOCKED) != string::npos) {
		/* door is locked, set the value to 1 */
		World::level().setDungeonSymbolValue(_position, DOOR_LOCKED);
	} else if (messages.find(MESSAGE_BREAK_SHOP_DOOR) != string::npos) {
		/* oops, we broke a shopkeepers door, better pay */
		World::setAction(static_cast<action::Action*> (new action::Answer(this, YES)));
	} else if (messages.find(MESSAGE_CLOSED_FOR_INVENTORY) != string::npos) {
		/* a shop that is closed for inventory */
		stack<Point> door;

		door.push(Point(Saiph::position().row() + 1, Saiph::position().col()));
		door.push(Point(Saiph::position().row() - 1, Saiph::position().col()));
		door.push(Point(Saiph::position().row(), Saiph::position().col() + 1));
		door.push(Point(Saiph::position().row(), Saiph::position().col() - 1));

		while (door.empty() == false) {
			Point top = door.top();
			door.pop();

			if (World::level().tile(top).symbol() == CLOSED_DOOR) {
				Debug::custom(name()) << "Marking " << top << " as DOOR_SHOP_INVENTORY" << endl;
				World::level().setDungeonSymbol(top, DOOR_SHOP_INVENTORY);
				break;
			}
		}
	}
}

void Door::onEvent(Event * const event) {
	if (event->id() == ChangedInventoryItems::ID) {
		/* inventory changed, see if we lost our unlocking device or got a new/better one */
		map<unsigned char, Item>::iterator i = Inventory::items().find(_unlock_tool_key);
		if (Inventory::items().find(_unlock_tool_key) == Inventory::items().end())
			_unlock_tool_key = 0; // darn, we lost our unlocking device
		ChangedInventoryItems* e = static_cast<ChangedInventoryItems*> (event);
		for (set<unsigned char>::iterator k = e->keys().begin(); k != e->keys().end(); ++k) {
			map<unsigned char, Item>::iterator i = Inventory::items().find(*k);
			if (i != Inventory::items().end() && wantItem(i->second))
				_unlock_tool_key = *k; // better key than what we currently got
		}
	} else if (event->id() == ReceivedItems::ID) {
		ReceivedItems* e = static_cast<ReceivedItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if (wantItem(i->second))
				_unlock_tool_key = i->first; // better key than what we currently got
		}
	} else if (event->id() == WantItems::ID) {
		WantItems* e = static_cast<WantItems*> (event);
		for (map<unsigned char, Item>::iterator i = e->items().begin(); i != e->items().end(); ++i) {
			if ((_unlock_tool_key != ILLEGAL_ITEM && i->first == _unlock_tool_key) || wantItem(i->second))
				i->second.want(i->second.count());
		}
	}
}

/* private methods */
bool Door::wantItem(const Item& item) {
	map<const string, const data::Key*>::const_iterator k = data::Key::keys().find(item.name());
	if (k == data::Key::keys().end())
		return false; // not a key

	map<unsigned char, Item>::iterator i = Inventory::items().find(_unlock_tool_key);
	if (i == Inventory::items().end())
		return true; // we got no key, we want this key

	/* simple check to make us keep the best unlocking tool.
	 * this needs to be improved, though. doesn't care about beatitude */
	if (i->second.name() == "Master Key of Thievery")
		return false;
	if (item.name() == "Master Key of Thievery")
		return true;
	if (i->second.name() == "skeleton key" || i->second.name() == "key")
		return false;
	if (item.name() == "skeleton key" || item.name() == "key")
		return true;
	if (i->second.name() == "lock pick")
		return false;
	if (item.name() == "lock pick")
		return true;
	if (i->second.name() == "credit card")
		return false;
	return true;
}
