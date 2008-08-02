#include "Door.h"

/* constructors */
Door::Door(Saiph *saiph) : Analyzer("Door"), saiph(saiph), command2(""), cur_door(-1, -1) {
}

/* methods */
void Door::finish() {
	/* open closed doors */
	if (saiph->best_priority > DOOR_OPEN_PRIORITY)
		return; // another analyzer got higher priority than we ever will have
	int least_moves = INT_MAX;
	for (map<Point, int>::iterator d = saiph->levels[saiph->position.level].symbols[CLOSED_DOOR].begin(); d != saiph->levels[saiph->position.level].symbols[CLOSED_DOOR].end(); ++d) {
		int moves = -1;
		unsigned char move = saiph->shortestPath(d->first, true, &moves);
		if (move == ILLEGAL_MOVE)
			continue;
		cur_door = d->first;
		if (moves == 1) {
			/* open/pick/kick door */
			if (d->second != 1)
				command = OPEN;
			else
				command = KICK;
			command2 = move;
			priority = DOOR_OPEN_PRIORITY;
			return;
		} else if (moves < least_moves) {
			/* go to door */
			command = move;
			command2 = "";
			priority = DOOR_OPEN_PRIORITY;
			least_moves = moves;
		}
	}
	if (least_moves < INT_MAX)
		return;
	cur_door.row = -1;
	cur_door.col = -1;
	command = "";
	command2 = "";
	priority = ILLEGAL_PRIORITY;
}

void Door::parseMessages(const string &messages) {
	if (command2 != "" && messages.find(MESSAGE_CHOOSE_DIRECTION, 0) != string::npos) {
		/* which direction we should open/pick/kick */
		priority = PRIORITY_CONTINUE_ACTION;
		command = command2;
	} else if (messages.find(MESSAGE_DOOR_LOCKED, 0) != string::npos) {
		/* door is locked, set the value to 1 */
		saiph->levels[saiph->position.level].setDungeonSymbol(cur_door, CLOSED_DOOR, 1);
	}
}
