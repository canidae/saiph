#include "Door.h"

/* constructors */
Door::Door(Saiph *saiph) : Analyzer("Door"), saiph(saiph), command2("") {
}

/* methods */
void Door::complete() {
	if (command.size() == 1 && command[0] == KICK)
		command = "";
}

void Door::finish() {
	/* open closed doors */
	if (saiph->best_priority > DOOR_OPEN_PRIORITY)
		return; // another analyzer got higher priority than we ever will have
	if (command.size() == 1 && command[0] == KICK)
		return; // kicking down door
	int least_moves = INT_MAX;
	for (map<Point, int>::iterator d = saiph->dungeon_feature[CLOSED_DOOR][saiph->position.branch][saiph->position.level].begin(); d != saiph->dungeon_feature[CLOSED_DOOR][saiph->position.branch][saiph->position.level].end(); ++d) {
		int moves = -1;
		unsigned char move = saiph->shortestPath(d->first, true, &moves);
		if (move == ILLEGAL_MOVE)
			continue;
		if (moves == 1) {
			/* open door */
			command = OPEN;
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
		/* kick it down, for now */
		command = KICK;
		priority = DOOR_OPEN_PRIORITY;
	}
}
