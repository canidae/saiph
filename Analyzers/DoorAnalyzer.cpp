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
				doors[b][du][d].open = false;
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

int DoorAnalyzer::analyze(int row, int col, char symbol) {
	int branch = saiph->current_branch;
	int dungeon = saiph->world->player.dungeon;
	for (int d = 0; d < DO_MAX_DOORS; ++d) {
		if (doors[branch][dungeon][d].row == -1) {
			doors[branch][dungeon][d].row = row;
			doors[branch][dungeon][d].col = col;
			doors[branch][dungeon][d].open = (symbol == OPEN_DOOR);
			doors[branch][dungeon][d].locked = false;
			doors[branch][dungeon][d].shopkeeper = false;
			return 0;
		} else if (doors[branch][dungeon][d].row == row && doors[branch][dungeon][d].col == col) {
			doors[branch][dungeon][d].open = (symbol == OPEN_DOOR);
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
	int shortest_distance = -1;
	int best_door = -1;
	char best_move = -1;
	int best_priority = 0;
	for (int d = 0; d < DO_MAX_DOORS; ++d) {
		int to_row = doors[branch][dungeon][d].row;
		if (to_row == -1)
			continue;
		int to_col = doors[branch][dungeon][d].col;
		if (saiph->branches[branch]->map[dungeon][to_row][to_col] != OPEN_DOOR || saiph->branches[branch]->map[dungeon][to_row][to_col] != CLOSED_DOOR) {
			/* this door seems to be gone */
			doors[branch][dungeon][d].row = -1;
			continue;
		}
		if (doors[branch][dungeon][d].open)
			continue;
		if (doors[branch][dungeon][d].locked && shop_on_level[branch][dungeon])
			continue; // FIXME: currently ignoring all locked doors on levels there are shopkeepers on
		/* see if we're already where we want to be */
		if ((row == to_row - 1 && col == to_col) || (row == to_row + 1 && col == to_col) || (row == to_row && col == to_col - 1) || (row == to_row && col == to_col + 1)) {
			if (doors[branch][dungeon][d].locked) {
				if (DO_KICK_DOOR > best_priority) {
					best_door = d;
					best_move = saiph->moveToDirection(to_row, to_col, row, col);
					shortest_distance = 0;
					best_priority = DO_KICK_DOOR;
					break;
				}
			} else {
				if (DO_OPEN_DOOR > best_priority) {
					best_door = d;
					best_move = saiph->moveToDirection(to_row, to_col, row, col);
					shortest_distance = 0;
					best_priority = DO_OPEN_DOOR;
					break;
				}
			}
		}
		if (DO_SEEK_DOOR < best_priority)
			continue;
		/* we want to move directly in front of doors (in case there's a shop here) */
		bool direct_line = false;
		int distance = -1;
		char move = saiph->shortestPath(to_row - 1, to_col, false, distance, direct_line);
		if (move == -1)
			move = saiph->shortestPath(to_row + 1, to_col, false, distance, direct_line);
		if (move == -1)
			move = saiph->shortestPath(to_row, to_col - 1, false, distance, direct_line);
		if (move == -1)
			move = saiph->shortestPath(to_row, to_col + 1, false, distance, direct_line);
		if (move == -1)
			continue; // unable to find path to door
		if (distance < shortest_distance) {
			best_door = d;
			best_move = move;
			shortest_distance = distance;
			best_priority = DO_SEEK_DOOR;
		}
	}
	if (best_door == -1)
		return 0;
	action_door = best_door;
	action_direction = best_move;
	if (shortest_distance == 0) {
		/* we're next to a door */
		if (!doors[branch][dungeon][best_door].locked) {
			/* attempt to open it */
			action = DO_OPEN_DOOR;
		} else {
			/* TODO
			 * lock picking */
			/* kick it down */
			action = DO_KICK_DOOR;
		}
	} else {
		/* we see a closed door, but we'll have to walk a bit */
		action = DO_SEEK_DOOR;
	}
	return best_priority;
}

void DoorAnalyzer::command() {
	char command[2];
	cerr << action << endl;
	cerr << action_direction << endl;
	switch (action) {
		case DO_NOTHING:
			return;
			break;

		case DO_CHOOSE_DIRECTION:
			command[0] = action_direction;
			break;

		case DO_KICK_DOOR:
			command[0] = KICK;
			break;

		case DO_PICK_DOOR:
			/* TODO */
			cerr << "attempting to pick door, but i don't know how to pick doors yet" << endl;
			exit(1);
			break;

		case DO_OPEN_DOOR:
			command[0] = OPEN;
			break;

		case DO_SEEK_DOOR:
			command[0] = action_direction;
			break;

		default:
			cerr << "undefined action in DoorAnalyzer: " << action << endl;
			exit(1);
			break;
	}
	command[1] = '\0';
	saiph->world->command(command);
}
