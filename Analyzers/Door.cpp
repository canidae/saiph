#include "Door.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Door::Door(Saiph *saiph) : Analyzer("Door"), saiph(saiph), command2(""), unlock_tool_key(0), in_a_pit(false) {
}

/* methods */
void Door::analyze() {
	/* open closed doors */
	if (saiph->world->player.hurt_leg)
		return; // don't open doors when our leg is hurt
	else if (saiph->world->player.blind)
		return; // don't move when blind
	if (in_a_pit) {
		/* we're possibly in a pit.
		 * most common way to get out of pits is to crawl out,
		 * but she might get teleported out or levitate out.
		 * so we'll check if her position has changed, if it has
		 * then she's probably no longer in a pit */
		if (position != saiph->position)
			in_a_pit = false; // no longer in the pit
		else
			return; // still in the pit
	}
	/* go to nearest closed door and get it open somehow */
	int least_moves = INT_MAX;
	for (map<Point, int>::iterator d = saiph->levels[saiph->position.level].symbols[(unsigned char) CLOSED_DOOR].begin(); d != saiph->levels[saiph->position.level].symbols[(unsigned char) CLOSED_DOOR].end(); ++d) {
		if (saiph->levels[saiph->position.level].branch == BRANCH_MINES && d->second == 1) {
			/* don't kick/pick doors when we're in the mines */
			findUnlockingTool();
			if (unlock_tool_key == 0 || (saiph->inventory[unlock_tool_key].name != "skeleton key" && saiph->inventory[unlock_tool_key].name != "key"))
				continue; // no key in inventory
		}
		int moves = -1;
		unsigned char dir = saiph->shortestPath(d->first, true, &moves);
		if (dir == ILLEGAL_DIRECTION)
			continue;
		if (moves == 1) {
			/* open/pick/kick door */
			if (d->second != 1) {
				command = OPEN;
			} else {
				findUnlockingTool();
				/* we can't apply when we're overtaxed, but can still kick... */
				if (unlock_tool_key == 0 || saiph->world->player.encumbrance >= OVERTAXED)
					command = KICK;
				else
					command = APPLY;
			}
			command2 = dir;
			position = d->first;
			priority = PRIORITY_DOOR_OPEN;
			return;
		} else if (moves < least_moves) {
			/* go to door */
			command = dir;
			priority = PRIORITY_DOOR_OPEN;
			least_moves = moves;
		}
	}
	if (least_moves < INT_MAX)
		return;
	priority = ILLEGAL_PRIORITY;
}

void Door::parseMessages(const string &messages) {
	if (saiph->world->question && messages.find(MESSAGE_WHAT_TO_APPLY, 0) != string::npos) {
		/* what to apply */
		command = unlock_tool_key;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (messages.find(MESSAGE_CHOOSE_DIRECTION, 0) != string::npos) {
		/* which direction we should open/pick/kick */
		command = command2;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (saiph->world->question && messages.find(DOOR_UNLOCK_IT, 0) != string::npos) {
		/* unlock door? */
		command = YES;
		priority = PRIORITY_CONTINUE_ACTION;
		/* we're going to assume the door won't be locked anymore */
		saiph->levels[saiph->position.level].symbols[(unsigned char) CLOSED_DOOR][position] = UNKNOWN_SYMBOL_VALUE;
	} else if (messages.find(DOOR_DOOR_LOCKED, 0) != string::npos) {
		/* door is locked, set the value to 1 */
		saiph->levels[saiph->position.level].symbols[(unsigned char) CLOSED_DOOR][position] = 1;
	} else if (messages.find(DOOR_BREAK_SHOP_DOOR, 0) != string::npos) {
		/* oops, we broke a shopkeepers door, better pay */
		command = YES;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (messages.find(MESSAGE_CANT_REACH_OVER_PIT, 0) != string::npos) {
		/* we're in a pit, can't reach door from here */
		in_a_pit = true;
		position = saiph->position;
	} else if (messages.find(MESSAGE_CRAWL_OUT_OF_PIT, 0) != string::npos) {
		/* crawled out of pit */
		in_a_pit = false;
	}
}

/* private methods */
void Door::findUnlockingTool() {
	/* find [skeleton] key, lock pick or credit card.
	 * we'll assume that we only carry 1 such item, and that we only carry the best item */
	map<unsigned char, Item>::iterator k = saiph->inventory.find(unlock_tool_key);
	if (k != saiph->inventory.end() && (k->second.name == "skeleton key" || k->second.name == "key" || k->second.name == "lock pick" || k->second.name == "credit card"))
		return;
	for (k = saiph->inventory.begin(); k != saiph->inventory.end(); ++k) {
		if (k->second.name != "skeleton key" && k->second.name != "key" && k->second.name != "lock pick" && k->second.name != "credit card")
			continue;
		/* this should be a unlocking device */
		unlock_tool_key = k->first;
		return;
	}
	/* no tool for unlocking doors */
	unlock_tool_key = 0;
}
