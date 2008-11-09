#include "Door.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Door::Door(Saiph *saiph) : Analyzer("Door"), saiph(saiph), unlock_tool_key(0) {
}

/* methods */
void Door::analyze() {
	/* open closed doors */
	if (saiph->world->player.hurt_leg)
		return; // don't open doors when our leg is hurt
	/* go to nearest closed door and get it open somehow */
	int least_moves = INT_MAX;
	for (map<Point, int>::iterator d = saiph->levels[saiph->position.level].symbols[CLOSED_DOOR].begin(); d != saiph->levels[saiph->position.level].symbols[CLOSED_DOOR].end(); ++d) {
		int moves = -1;
		unsigned char move = saiph->shortestPath(d->first, true, &moves);
		if (move == ILLEGAL_MOVE)
			continue;
		if (moves == 1) {
			/* open/pick/kick door */
			if (d->second != 1) {
				setCommand(0, DOOR_OPEN_PRIORITY, OPEN);
				setCommand(1, PRIORITY_CONTINUE_ACTION, string(move, 1));
			} else {
				findUnlockingTool();
				if (unlock_tool_key == 0) {
					setCommand(0, DOOR_OPEN_PRIORITY, KICK);
					setCommand(1, PRIORITY_CONTINUE_ACTION, string(move, 1));
				} else {
					setCommand(0, DOOR_OPEN_PRIORITY, APPLY);
					setCommand(1, PRIORITY_CONTINUE_ACTION, string(unlock_tool_key, 1));
					setCommand(2, PRIORITY_CONTINUE_ACTION, string(move, 1));
					setCommand(3, PRIORITY_CONTINUE_ACTION, YES); // yes, unlock the door
				}
			}
			cur_door = d->first;
			sequence = 0;
			return;
		} else if (moves < least_moves) {
			/* go to door */
			setCommand(0, DOOR_OPEN_PRIORITY, string(move, 1));
			sequence = 0;
			least_moves = moves;
		}
	}
}

void Door::complete() {
	if (sequence == 0)
		sequence = 1;
}

void Door::parseMessages(const string &messages) {
	if (sequence == 0 && saiph->world->question && messages.find(MESSAGE_WHAT_TO_APPLY, 0) != string::npos) {
		/* what to apply */
		++sequence;
	} else if (sequence > 0 && messages.find(MESSAGE_CHOOSE_DIRECTION, 0) != string::npos) {
		/* which direction we should open/pick/kick */
		++sequence;
	} else if (sequence == 2 && saiph->world->question && messages.find(MESSAGE_UNLOCK_IT, 0) != string::npos) {
		/* unlock door? */
		++sequence;
		/* we're going to assume the door won't be locked anymore */
		saiph->levels[saiph->position.level].setDungeonSymbolValue(cur_door, UNKNOWN_SYMBOL_VALUE);
		sequence = -1;
	} else if (messages.find(MESSAGE_DOOR_LOCKED, 0) != string::npos) {
		/* door is locked, set the value to 1 */
		saiph->levels[saiph->position.level].setDungeonSymbolValue(cur_door, 1);
	} else if (sequence > 0) {
		/* only first action is an action that may have lower priority
		 * than the action from another analyzer.
		 * we can safely set sequence back to -1 */
		sequence = -1;
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
