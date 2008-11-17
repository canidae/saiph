#include "Door.h"
#include "../Saiph.h"
#include "../World.h"

using namespace std;

/* constructors/destructor */
Door::Door(Saiph *saiph) : Analyzer("Door"), saiph(saiph), command2(""), sequence(-1), unlock_tool_key(0) {
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
		unsigned char dir = saiph->shortestPath(d->first, true, &moves);
		if (dir == ILLEGAL_DIRECTION)
			continue;
		if (moves == 1) {
			/* open/pick/kick door */
			if (d->second != 1) {
				command = OPEN;
			} else {
				findUnlockingTool();
				if (unlock_tool_key == 0)
					command = KICK;
				else
					command = APPLY;
			}
			command2 = dir;
			cur_door = d->first;
			priority = DOOR_OPEN_PRIORITY;
			sequence = 0;
			return;
		} else if (moves < least_moves) {
			/* go to door */
			command = MOVE;
			command.push_back(dir);
			priority = DOOR_OPEN_PRIORITY;
			least_moves = moves;
		}
	}
	if (least_moves < INT_MAX)
		return;
	priority = ILLEGAL_PRIORITY;
}

void Door::complete() {
	if (sequence == 0)
		sequence = 1;
}

void Door::parseMessages(const string &messages) {
	if (sequence == 1 && saiph->world->question && messages.find(MESSAGE_WHAT_TO_APPLY, 0) != string::npos) {
		/* what to apply */
		command = unlock_tool_key;
		priority = PRIORITY_CONTINUE_ACTION;
	} else if (sequence == 1 && messages.find(MESSAGE_CHOOSE_DIRECTION, 0) != string::npos) {
		/* which direction we should open/pick/kick */
		command = command2;
		priority = PRIORITY_CONTINUE_ACTION;
		sequence = 2;
	} else if (sequence == 2 && saiph->world->question && messages.find(DOOR_UNLOCK_IT, 0) != string::npos) {
		/* unlock door? */
		command = YES;
		priority = PRIORITY_CONTINUE_ACTION;
		/* we're going to assume the door won't be locked anymore */
		saiph->levels[saiph->position.level].setDungeonSymbolValue(cur_door, UNKNOWN_SYMBOL_VALUE);
		sequence = -1;
	} else if (messages.find(DOOR_DOOR_LOCKED, 0) != string::npos) {
		/* door is locked, set the value to 1 */
		saiph->levels[saiph->position.level].setDungeonSymbolValue(cur_door, 1);
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
