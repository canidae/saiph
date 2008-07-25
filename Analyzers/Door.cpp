#include "Door.h"

/* constructors */
Door::Door(Saiph *saiph) : Analyzer("Door"), saiph(saiph) {
	memset(shop_on_level, false, sizeof (shop_on_level));
	command_door = NULL;
}

/* methods */
void Door::complete() {
}

void Door::finish() {
	/* open closed doors */
	if (priority > DOOR_OPEN_PRIORITY)
		return;
	command = "";
	command2 = "";
	command_door = NULL;
	int best_distance = INT_MAX;
	for (vector<DoorPoint>::iterator d = doors[saiph->position.branch][saiph->position.level].begin(); d != doors[saiph->position.branch][saiph->position.level].end(); ) {
		/* check if door still is there */
		if (saiph->dungeonmap[saiph->position.branch][saiph->position.level][d->row][d->col] != OPEN_DOOR && saiph->dungeonmap[saiph->position.branch][saiph->position.level][d->row][d->col] != CLOSED_DOOR) {
			/* it isn't, remove it from the list */
			d = doors[saiph->position.branch][saiph->position.level].erase(d);
			continue;
		}
		if (saiph->dungeonmap[saiph->position.branch][saiph->position.level][d->row][d->col] == OPEN_DOOR) {
			/* door is open, currently uninteresting */
			++d;
			continue;
		}
		/*
		if (d->locked && shop_on_level[saiph->position.branch][saiph->position.level]) {
			++d;
			continue;
		}
		*/
		bool straight_line = false;
		int distance = -1;
		unsigned char move = saiph->shortestPath(*d, true, &distance, &straight_line);
		if (move == ILLEGAL_MOVE) {
			/* can't path to this door, ignore it */
			++d;
			continue;
		}
		if (!d->locked && DOOR_OPEN_PRIORITY >= priority) {
			/* [go to] open a door */
			if (DOOR_OPEN_PRIORITY == priority && distance >= best_distance) {
				/* another door we want to open is nearer */
				++d;
				continue;
			}
			priority = DOOR_OPEN_PRIORITY;
			if (distance == 1) {
				command = OPEN; // standing next to door
				command2 = move;
				command_door = &(*d);
			} else {
				command = move;
			}
		/* TODO:
		} else if (d->locked && can_lockpick && DOOR_PICK_PRIORITY >= priority) {
		*/
			/* [go to] pick a door */
			/*
			if (DOOR_PICK_PRIORITY == priority && distance >= best_distance) {
			*/
				/* another door we want to pick is nearer */
				/*
				++d;
				continue;
			}
			priority = DOOR_PICK_PRIORITY;
		*/
		} else if (d->locked && DOOR_KICK_PRIORITY >= priority) {
			/* [go to] kick a door */
			if (DOOR_KICK_PRIORITY == priority && distance >= best_distance) {
				/* another door we want to kick is nearer */
				++d;
				continue;
			}
			priority = DOOR_KICK_PRIORITY;
			if (distance == 1) {
				command = KICK; // standing next to door
				command2 = move;
				command_door = &(*d);
			} else {
				command = move;
			}
		}
		++d;
	}
}

void Door::inspect(const Point &point) {
	if ((unsigned char) saiph->world->view[point.row][point.col] != OPEN_DOOR && (unsigned char) saiph->world->view[point.row][point.col] != CLOSED_DOOR)
		return; // we only care about doors
	for (vector<DoorPoint>::iterator d = doors[saiph->position.branch][saiph->position.level].begin(); d != doors[saiph->position.branch][saiph->position.level].end(); ++d) {
		if (d->row == point.row && d->col == point.col)
			return; // we already know about this door
	}
	/* add this door to the list */
	DoorPoint dp;
	dp.row = point.row;
	dp.col = point.col;
	dp.locked = false;
	doors[saiph->position.branch][saiph->position.level].push_back(dp);
}

void Door::parseMessages(const string &messages) {
	if (command2 != "" && messages.find(MESSAGE_CHOOSE_DIRECTION, 0) != string::npos) {
		priority = PRIORITY_CONTINUE_ACTION;
		command = command2;
		command2 = "";
		return;
	//} else if (messages.find(MESSAGE_DOOR_CLOSES, 0) != string::npos) {
	//} else if (messages.find(MESSAGE_KICK_DOOR_FAIL, 0) != string::npos) {
	//} else if (messages.find(MESSAGE_KICK_DOOR_OPEN, 0) != string::npos) {
	} else if (messages.find(MESSAGE_DOOR_LOCKED, 0) != string::npos && command_door != NULL) {
		command_door->locked = true;
	//} else if (messages.find(MESSAGE_NO_DOOR, 0) != string::npos) {
	//} else if (messages.find(MESSAGE_DOOR_OPENS, 0) != string::npos) {
	//} else if (messages.find(MESSAGE_DOOR_RESISTS, 0) != string::npos) {
	} else if (!shop_on_level[saiph->position.branch][saiph->position.level] && messages.find(MESSAGE_SHOP_ON_LEVEL1, 0) != string::npos || messages.find(MESSAGE_SHOP_ON_LEVEL2, 0) != string::npos || messages.find(MESSAGE_SHOP_ON_LEVEL3, 0) != string::npos) {
		/* TODO
		 * this will not be specific for doors(?) */
		shop_on_level[saiph->position.branch][saiph->position.level] = true;
	}
}
