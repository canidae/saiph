#include "Door.h"

/* constructors */
Door::Door(Saiph *saiph) {
	this->saiph = saiph;
	vector<unsigned char> symbols;
	symbols.push_back(OPEN_DOOR);
	symbols.push_back(CLOSED_DOOR);
	saiph->registerAnalyzerSymbols(this, symbols);
}

/* methods */
void Door::command(string *command) {
	*command = action;
	action = "OK";
}

int Door::finish() {
	/* open closed doors */
	action = "";
	int b = saiph->current_branch;
	int l = saiph->current_level;
	int best_priority = 0;
	int best_distance = INT_MAX;
	for (vector<DoorPoint>::iterator d = doors[b][l].begin(); d != doors[b][l].end(); ) {
		/* check if door still is there */
		if (saiph->map[b][l].dungeon[d->row][d->col] != OPEN_DOOR && saiph->map[b][l].dungeon[d->row][d->col] != CLOSED_DOOR) {
			/* it isn't, remove it from the list */
			d = doors[b][l].erase(d);
			continue;
		}
		if (saiph->map[b][l].dungeon[d->row][d->col] == OPEN_DOOR) {
			/* door is open, currently uninteresting */
			++d;
			continue;
		}
		if (d->locked && shop_on_level[b][l]) {
			/* currently we'll ignore every locked door on levels with shops */
			++d;
			continue;
		}
		bool straight_line = false;
		int distance = -1;
		unsigned char move = saiph->shortestPath(*d, true, &distance, &straight_line);
		if (move == ILLEGAL_MOVE) {
			/* can't path to this door, ignore it */
			++d;
			continue;
		}
		if (!d->locked && DOOR_OPEN_PRIORITY >= best_priority) {
			/* [go to] open a door */
			if (DOOR_OPEN_PRIORITY == best_priority && distance >= best_distance) {
				/* another door we want to open is nearer */
				++d;
				continue;
			}
			best_priority = DOOR_OPEN_PRIORITY;
			if (distance == 1) {
				/* standing next to door */
				action = "o";
				next_action = move;
			} else {
				action = move;
			}
		/* TODO:
		} else if (d->locked && can_lockpick && DOOR_PICK_PRIORITY >= best_priority) {
		*/
			/* [go to] pick a door */
			/*
			if (DOOR_PICK_PRIORITY == best_priority && distance >= best_distance) {
			*/
				/* another door we want to pick is nearer */
				/*
				++d;
				continue;
			}
			best_priority = DOOR_PICK_PRIORITY;
		*/
		} else if (d->locked && DOOR_KICK_PRIORITY >= best_priority) {
			/* [go to] kick a door */
			if (DOOR_KICK_PRIORITY == best_priority && distance >= best_distance) {
				/* another door we want to kick is nearer */
				++d;
				continue;
			}
			best_priority = DOOR_KICK_PRIORITY;
			if (distance == 1) {
				/* standing next to door */
				action = (char) 4;
				next_action = move;
			} else {
				action = move;
			}
		}
		++d;
	}
	return best_priority;
}

void Door::inspect(int row, int col, unsigned char symbol) {
	int b = saiph->current_branch;
	int l = saiph->current_level;
	for (vector<DoorPoint>::iterator d = doors[b][l].begin(); d != doors[b][l].end(); ++d) {
		if (d->row == row && d->col == col)
			return; // we already know about this door
	}
	/* add this door to the list */
	DoorPoint dp;
	dp.row = row;
	dp.col = col;
	dp.locked = false;
	doors[b][l].push_back(dp);
}

int Door::parseMessages(string *messages) {
	if (action != "OK")
		return 0; // we didn't do anything
	if (messages->find(CHOOSE_DIRECTION, 0) != string::npos) {
		action = next_action;
		return DOOR_CONTINUE_ACTION;
	//} else if (messages->find(DOOR_CLOSES, 0) != string::npos) {
	//} else if (messages->find(DOOR_KICK_FAIL, 0) != string::npos) {
	//} else if (messages->find(DOOR_KICK_OPEN, 0) != string::npos) {
	} else if (messages->find(DOOR_LOCKED, 0) != string::npos) {
		doors[saiph->current_branch][saiph->current_level][action_door].locked = true;
	//} else if (messages->find(DOOR_NO_DOOR, 0) != string::npos) {
	//} else if (messages->find(DOOR_OPENS, 0) != string::npos) {
	//} else if (messages->find(DOOR_RESISTS, 0) != string::npos) {
	} else if (messages->find(DOOR_SHOP_ON_LEVEL1, 0) != string::npos || messages->find(DOOR_SHOP_ON_LEVEL2, 0) != string::npos || messages->find(DOOR_SHOP_ON_LEVEL3, 0) != string::npos) {
		/* TODO
		 * this will not be specific for doors(?) */
		shop_on_level[saiph->current_branch][saiph->current_level] = true;
	}
	return 0;
}
