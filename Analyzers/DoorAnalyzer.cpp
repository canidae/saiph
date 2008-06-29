#include "DoorAnalyzer.h"

/* constructors */
DoorAnalyzer::DoorAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	for (int b = 0; b < MAX_BRANCHES; ++b) {
		for (int du = 0; du < MAX_DUNGEON_DEPTH; ++du) {
			shop_on_level[b][du] = false;
			for (int d = 0; d < DO_MAX_DOORS; ++d) {
				doors[b][du][d].row = -1;
				doors[b][du][d].col = -1;
				doors[b][du][d].locked = false;
				doors[b][du][d].shopkeeper = false;
			}
		}
	}
	action = DO_NOTHING;

	symbols[symbol_count++] = OPEN_DOOR;
	symbols[symbol_count++] = CLOSED_DOOR;
}

/* methods */
int DoorAnalyzer::parseMessages(string *messages) {
	if (messages->find(CHOOSE_DIRECTION, 0) != string::npos && (action == DO_KICK_DOOR || action == DO_OPEN_DOOR || action == DO_PICK_DOOR)) {
		action = DO_CHOOSE_DIRECTION;
		return 100;
	//} else if (messages->find(DO_CLOSES, 0) != string::npos) {
	//} else if (messages->find(DO_KICK_FAIL, 0) != string::npos) {
	//} else if (messages->find(DO_KICK_OPEN, 0) != string::npos) {
	} else if (messages->find(DO_LOCKED, 0) != string::npos) {
		doors[saiph->current_branch][saiph->world->player.dungeon][action_door].locked = true;
	//} else if (messages->find(DO_NO_DOOR, 0) != string::npos) {
	//} else if (messages->find(DO_OPENS, 0) != string::npos) {
	//} else if (messages->find(DO_RESISTS, 0) != string::npos) {
	//} else if (messages->find(FOOT_HURT, 0) != string::npos) {
		/* TODO
		 * move this to a "general" message parser */
	//} else if (messages->find(FOOT_UNHURT, 0) != string::npos) {
		/* TODO
		 * move this to a "general" message parser */
	} else if (messages->find(DO_SHOP_ON_LEVEL1, 0) != string::npos || messages->find(DO_SHOP_ON_LEVEL2, 0) != string::npos || messages->find(DO_SHOP_ON_LEVEL3, 0) != string::npos) {
		/* TODO
		 * this will not be specific for doors(?) */
		shop_on_level[saiph->current_branch][saiph->world->player.dungeon] = true;
	}
	action = DO_NOTHING;
	return 0;
}

int DoorAnalyzer::analyze(int row, int col, unsigned char symbol) {
	int branch = saiph->current_branch;
	int dungeon = saiph->world->player.dungeon;
	for (int d = 0; d < DO_MAX_DOORS; ++d) {
		if (doors[branch][dungeon][d].row == -1) {
			/* FIXME
			 * when we kick down a door then other doors may be shifted in this list,
			 * which makes them lose the "locked" status and saiph will attempt to open the door again.
			 * not critical, just annoying */
			doors[branch][dungeon][d].row = row;
			doors[branch][dungeon][d].col = col;
			doors[branch][dungeon][d].locked = false;
			doors[branch][dungeon][d].shopkeeper = false;
			return 0;
		} else if (doors[branch][dungeon][d].row == row && doors[branch][dungeon][d].col == col) {
			return 0;
		}
	}
	return 0;
}

int DoorAnalyzer::finish() {
	int row = saiph->world->player.row;
	int col = saiph->world->player.col;
	if (row < MAP_ROW_START || row > MAP_ROW_END || col < 0 || col >= COLS)
		return 0;
	int branch = saiph->current_branch;
	int dungeon = saiph->world->player.dungeon;
	int best_priority = 0;
	for (int d = 0; d < DO_MAX_DOORS; ++d) {
		int to_row = doors[branch][dungeon][d].row;
		if (to_row == -1)
			continue;
		int to_col = doors[branch][dungeon][d].col;
		if (saiph->branches[branch]->map[dungeon][to_row][to_col] != OPEN_DOOR && saiph->branches[branch]->map[dungeon][to_row][to_col] != CLOSED_DOOR) {
			/* this door seems to be gone */
			doors[branch][dungeon][d].row = -1;
			continue;
		}
		if (saiph->branches[branch]->map[dungeon][to_row][to_col] == OPEN_DOOR)
			continue;
		if (doors[branch][dungeon][d].locked && shop_on_level[branch][dungeon])
			continue; // FIXME: currently ignoring all locked doors on levels there are shopkeepers on
		bool direct_line = false;
		int distance = -1;
		unsigned char move = saiph->shortestPath(to_row, to_col, true, distance, direct_line);
		if (move == ILLEGAL_MOVE)
			continue;
		if (!doors[branch][dungeon][d].locked && DO_OPEN_PRIORITY > best_priority) {
			/* [go to] open a door */
			if (distance == 1)
				action = DO_OPEN_DOOR;
			else
				action = DO_SEEK_DOOR;
			action_direction = move;
			action_door = d;
			best_priority = DO_OPEN_PRIORITY;
		/*
		} else if (doors[branch][dungeon][d].locked && got_something_to_pick_with && DO_PICK_PRIORITY > best_priority) {
			// [go to] pick a door
			if (distance == 1)
				action = DO_PICK_DOOR;
			else
				action = DO_SEEK_DOOR;
			action_direction = move;
			action_door = d;
			best_priority = DO_PICK_PRIORITY;
		*/
		} else if (doors[branch][dungeon][d].locked && DO_KICK_PRIORITY > best_priority) {
			/* [go to] kick down a door */
			if (distance == 1)
				action = DO_KICK_DOOR;
			else
				action = DO_SEEK_DOOR;
			action_direction = move;
			action_door = d;
			best_priority = DO_KICK_PRIORITY;
		}
	}
	return best_priority;
}

void DoorAnalyzer::command(string *command) {
	cerr << action << endl;
	cerr << action_direction << endl;
	switch (action) {
		case DO_NOTHING:
			return;

		case DO_CHOOSE_DIRECTION:
			command->push_back(action_direction);
			break;

		case DO_KICK_DOOR:
			command->push_back(KICK);
			break;

		case DO_PICK_DOOR:
			/* TODO */
			cerr << "attempting to pick door, but i don't know how to pick doors yet" << endl;
			exit(1);
			break;

		case DO_OPEN_DOOR:
			command->push_back(OPEN);
			break;

		case DO_SEEK_DOOR:
			command->push_back(action_direction);
			break;

		default:
			cerr << "undefined action in DoorAnalyzer: " << action << endl;
			exit(1);
			break;
	}
}
