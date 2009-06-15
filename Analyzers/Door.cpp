#include <stack>
#include "Door.h"
#include "../Debug.h"
#include "../Inventory.h"
#include "../Item.h"
#include "../Saiph.h"
#include "../World.h"
#include "../Actions/Answer.h"
#include "../Actions/Kick.h"
#include "../Actions/Loot.h"
#include "../Actions/Move.h"
#include "../Actions/Open.h"
#include "../Actions/Select.h"
#include "../Actions/Unlock.h"
#include "../Data/Key.h"
#include "../Events/Event.h"
#include "../Events/ChangedInventoryItems.h"
#include "../Events/ItemsOnGround.h"
#include "../Events/PickupItems.h"
#include "../Events/ReceivedItems.h"

using namespace analyzer;
using namespace event;
using namespace std;

/* constructors/destructor */
Door::Door() : Analyzer("Door"), unlock_tool_key(0), in_a_pit(false) {
}

/* methods */
void Door::analyze() {
	/* open closed doors */
	if (Saiph::blind)
		return; // don't move when blind
	if (in_a_pit) {
		/* we're possibly in a pit.
		 * most common way to get out of pits is to crawl out,
		 * but she might get teleported out or levitate out.
		 * so we'll check if her position has changed, if it has
		 * then she's probably no longer in a pit */
		if (position != Saiph::position)
			in_a_pit = false; // no longer in the pit
		else
			return; // still in the pit
	}

	/* go to nearest closed door and get it open somehow */
	unsigned int least_moves = UNREACHABLE;
	for (map<Point, int>::iterator d = World::levels[Saiph::position.level].symbols[(unsigned char) CLOSED_DOOR].begin(); d != World::levels[Saiph::position.level].symbols[(unsigned char) CLOSED_DOOR].end(); ++d) {
		const PathNode &node = World::shortestPath(d->first);
		if (node.cost == UNREACHABLE)
			continue; // can't reach this door
		if (World::levels[Saiph::position.level].branch == BRANCH_MINES && d->second == DOOR_LOCKED && (unlock_tool_key == 0 || Inventory::items[unlock_tool_key].name == "lock pick" || Inventory::items[unlock_tool_key].name == "credit card"))
			continue; // don't kick/pick doors when we're in the mines
		if (d->second == DOOR_SHOP_INVENTORY && unlock_tool_key == 0)
			continue; // shop and we got no means of opening it (well, except kicking)
		if (node.moves == 1) {
			/* open/pick/kick door */
			if (d->second != DOOR_LOCKED) {
				World::setAction(static_cast<action::Action *>(new action::Open(this, node.dir, PRIORITY_DOOR_OPEN)));
			} else {
				/* we can't apply when we're overtaxed, but logically we can kick... */
				if (unlock_tool_key == 0 || Saiph::encumbrance >= OVERTAXED) {
					if (Saiph::hurt_leg)
						continue; // can't kick, hurt leg
					else
						World::setAction(static_cast<action::Action *>(new action::Kick(this, node.dir, PRIORITY_DOOR_OPEN)));
				} else {
					World::setAction(static_cast<action::Action *>(new action::Unlock(this, unlock_tool_key, node.dir, PRIORITY_DOOR_OPEN)));
				}
			}
			position = d->first;
			return;
		} else if (node.moves < least_moves) {
			/* go to door */
			World::setAction(static_cast<action::Action *>(new action::Move(this, node.dir, action::Move::calculatePriority(PRIORITY_DOOR_OPEN, node.moves))));
			least_moves = node.moves;
		}
	}
}

void Door::parseMessages(const string &messages) {
	if (messages.find(DOOR_SUCCEED_UNLOCKING) != string::npos) {
		/* door unlocked */
		World::levels[Saiph::position.level].symbols[(unsigned char) CLOSED_DOOR][position] = UNKNOWN_SYMBOL_VALUE;
	} else if (messages.find(DOOR_DOOR_LOCKED, 0) != string::npos) {
		/* door is locked, set the value to 1 */
		World::levels[Saiph::position.level].symbols[(unsigned char) CLOSED_DOOR][position] = 1;
	} else if (messages.find(DOOR_BREAK_SHOP_DOOR, 0) != string::npos) {
		/* oops, we broke a shopkeepers door, better pay */
		World::setAction(static_cast<action::Action *>(new action::Answer(this, YES)));
	} else if (messages.find(MESSAGE_CANT_REACH_OVER_PIT, 0) != string::npos) {
		/* we're in a pit, can't reach door from here */
		in_a_pit = true;
		position = Saiph::position;
	} else if (messages.find(MESSAGE_CRAWL_OUT_OF_PIT, 0) != string::npos) {
		/* crawled out of pit */
		in_a_pit = false;
	} else if (messages.find(DOOR_CLOSED_FOR_INVENTORY, 0) != string::npos) {
		/* a shop that is closed for inventory */
		stack<Point> door;

		door.push(Point(Saiph::position.row + 1, Saiph::position.col));
		door.push(Point(Saiph::position.row - 1, Saiph::position.col));
		door.push(Point(Saiph::position.row, Saiph::position.col + 1));
		door.push(Point(Saiph::position.row, Saiph::position.col - 1));

		while (door.empty() == false) {
			Point top = door.top();
			door.pop();

			if (World::getDungeonSymbol(top) == CLOSED_DOOR) {
				Debug::analyzer(name) << "Marking " << top << " as DOOR_SHOP_INVENTORY" << endl;
				World::setDungeonSymbol(top, DOOR_SHOP_INVENTORY);
				break;
			}
		}
	}
}

void Door::onEvent(Event *const event) {
	if (event->getID() == ChangedInventoryItems::id) {
		/* inventory changed, see if we lost our unlocking device or got a new/better one */
		ChangedInventoryItems *e = static_cast<ChangedInventoryItems *>(event);
		map<unsigned char, Item>::iterator i = Inventory::items.find(unlock_tool_key);
		if (Inventory::items.find(unlock_tool_key) == Inventory::items.end())
			unlock_tool_key = 0; // darn, we lost our unlocking device
		for (set<unsigned char>::iterator k = e->keys.begin(); k != e->keys.end(); ++k) {
			map<unsigned char, Item>::iterator i2 = Inventory::items.find(*k);
		}
	} else if (event->getID() == ReceivedItems::id) {
	} else if (event->getID() == PickupItems::id) {
		PickupItems *e = static_cast<PickupItems *>(event);
		for (map<unsigned char, Item>::iterator i = e->items.begin(); i != e->items.end(); ++i) {
			if (wantItem(i->second))
				World::setAction(static_cast<action::Action *>(new action::Select(this, i->first)));
		}
	} else if (event->getID() == ItemsOnGround::id) {
		ItemsOnGround *e = static_cast<ItemsOnGround *>(event);
		for (list<Item>::iterator i = e->items.begin(); i != e->items.end(); ++i) {
			if (wantItem(*i))
				World::setAction(static_cast<action::Action *>(new action::Loot(this, PRIORITY_DOOR_LOOT)));
		}
	}
}

/* private methods */
void Door::findUnlockingTool() {
	/* find [skeleton] key, lock pick or credit card.
	 * we'll assume that we only carry 1 such item, and that we only carry the best item */
	map<unsigned char, Item>::iterator k = Inventory::items.find(unlock_tool_key);
	if (k != Inventory::items.end() && (k->second.name == "skeleton key" || k->second.name == "key" || k->second.name == "lock pick" || k->second.name == "credit card"))
		return;
	for (k = Inventory::items.begin(); k != Inventory::items.end(); ++k) {
		if (k->second.name != "skeleton key" && k->second.name != "key" && k->second.name != "lock pick" && k->second.name != "credit card")
			continue;
		/* this should be a unlocking device */
		unlock_tool_key = k->first;
		return;
	}
	/* no tool for unlocking doors */
	unlock_tool_key = 0;
}

bool Door::wantItem(const Item &item) {
	map<string, data::Key *>::iterator k = data::Key::keys.find(item.name);
	if (k == data::Key::keys.end())
		return false; // not a key

	map<unsigned char, Item>::iterator i = Inventory::items.find(unlock_tool_key);
	if (i == Inventory::items.end())
		return true; // we got no key, we want this key

	/* simple check to make us keep the best unlocking tool.
	 * this needs to be improved, though. doesn't care about beatitude */
	if (i->second.name == "Master Key of Thievery")
		return false;
	if (item.name == "Master Key of Thievery")
		return true;
	if (i->second.name == "skeleton key" || i->second.name == "key")
		return false;
	if (item.name == "skeleton key" || item.name == "key")
		return true;
	if (i->second.name == "lock pick")
		return false;
	if (item.name == "lock pick")
		return true;
	if (i->second.name == "credit card")
		return false;
	return true;
}
