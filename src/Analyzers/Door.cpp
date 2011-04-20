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
#include "Events/ShopDetected.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Door::Door() : Analyzer("Door"), _unlock_tool_key(0) {
	/* register events */
	EventBus::registerEvent(ChangedInventoryItems::ID, this);
	EventBus::registerEvent(ReceivedItems::ID, this);
	EventBus::registerEvent(WantItems::ID, this);
	EventBus::registerEvent(ShopDetected::ID, this);
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
		if (World::level().branch() == BRANCH_MINETOWN && (d->second & DOOR_IS_LOCKED) && (_unlock_tool_key == 0 || Inventory::items()[_unlock_tool_key].name() == "lock pick" || Inventory::items()[_unlock_tool_key].name() == "credit card"))
			continue; // don't kick/pick doors when we're in minetown
		if ((d->second & (DOOR_IS_SHOP | DOOR_IS_LOCKED)) == (DOOR_IS_SHOP | DOOR_IS_LOCKED) && _unlock_tool_key == 0)
			continue; // shop and we got no means of opening it (well, except kicking)
		if (Saiph::inAPit() && tile.distance() == 1)
			continue; // in a pit next to the door, prevent her from trying to open it (and fail)
		if (tile.distance() == 1) {
			/* open/pick/kick door */
			if (!(d->second & DOOR_IS_LOCKED)) {
				World::setAction(static_cast<action::Action*> (new action::Open(this, tile.direction(), PRIORITY_DOOR_OPEN)));
			} else if (_unlock_tool_key != 0) {
				if (Saiph::encumbrance() <= STRAINED)
					World::setAction(static_cast<action::Action*> (new action::Unlock(this, _unlock_tool_key, tile.direction(), PRIORITY_DOOR_OPEN)));
			} else {
				/* see if we can kick if no pick */
				if (Saiph::position().row() != d->first.row() && Saiph::position().col() != d->first.col()) {
					stack<Point> ps;
					ps.push(Point(Saiph::position().row(), d->first.col()));
					ps.push(Point(d->first.row(), Saiph::position().col()));
					Tile pst;
					for (; !ps.empty(); ps.pop()) {
						pst = World::shortestPath(ps.top());
						if (pst.symbol() != CORRIDOR) continue;
						if (pst.search() == TILE_FULLY_SEARCHED) continue;
						// Try to stand orthogonally next to doors before kicking them, to catch "closed for inventory"
						World::setAction(static_cast<action::Action*> (new action::Move(this, pst, PRIORITY_DOOR_OPEN)));
						return;
					}
				}
				if (!Saiph::hurtLeg() && (Saiph::encumbrance() < STRAINED)) // can only kick at less than strained
					World::setAction(static_cast<action::Action*> (new action::Kick(this, tile.direction(), PRIORITY_DOOR_OPEN)));
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
		World::level().setDungeonSymbolValue(_position, getDoorFlags(_position) & ~DOOR_IS_LOCKED);
	} else if (messages.find(MESSAGE_DOOR_LOCKED) != string::npos) {
		/* door is locked, set the value to 1 */
		World::level().setDungeonSymbolValue(_position, getDoorFlags(_position) | DOOR_IS_LOCKED);
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
				World::level().setDungeonSymbolValue(top, getDoorFlags(top) | DOOR_IS_SHOP);
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
	} else if (event->id() == ShopDetected::ID) {
		ShopDetected* e = static_cast<ShopDetected*> (event);
		stack<Point> ps;
		for (int x = e->ul().col(); x <= e->lr().col(); ++x) {
			ps.push(Point(e->ul().row() - 1, x));
			ps.push(Point(e->lr().row() + 1, x));
		}
		for (int y = e->ul().row() - 1; y <= e->lr().row() + 1; ++y) {
			ps.push(Point(y, e->ul().col() - 1));
			ps.push(Point(y, e->lr().col() + 1));
		}
		for (; !ps.empty(); ps.pop()) {
			if (World::level().tile(ps.top()).symbol() != CLOSED_DOOR)
				continue;
			World::level().setDungeonSymbolValue(ps.top(), getDoorFlags(ps.top()) | DOOR_IS_SHOP);
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

int Door::getDoorFlags(const Point& pt) {
	map<Point, int>::const_iterator sp = World::level().symbols(CLOSED_DOOR).find(pt);
	if (sp == World::level().symbols(CLOSED_DOOR).end())
		return 0;
	if (sp->second == UNKNOWN_SYMBOL_VALUE)
		return 0;
	return sp->second;
}
