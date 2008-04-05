#include "DoorAnalyzer.h"

/* constructors */
DoorAnalyzer::DoorAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
}

/* methods */

void DoorAnalyzer::finish() {
	int row = saiph->world->player.row;
	int col = saiph->world->player.col;
	if (row < MAP_ROW_START || row > MAP_ROW_END || col < 0 || col >= COLS)
		return;
	int branch = saiph->current_branch;
	int dungeon = saiph->world->player.dungeon;
	char kick = -1;
	if (saiph->branches[branch]->map[dungeon][row + 1][col + 1] == CLOSED_DOOR)
		kick = MOVE_SE;
	else if (saiph->branches[branch]->map[dungeon][row + 1][col - 1] == CLOSED_DOOR)
		kick = MOVE_SW;
	else if (saiph->branches[branch]->map[dungeon][row - 1][col + 1] == CLOSED_DOOR)
		kick = MOVE_NE;
	else if (saiph->branches[branch]->map[dungeon][row - 1][col - 1] == CLOSED_DOOR)
		kick = MOVE_NW;
	else if (saiph->branches[branch]->map[dungeon][row + 1][col] == CLOSED_DOOR)
		kick = MOVE_S;
	else if (saiph->branches[branch]->map[dungeon][row - 1][col] == CLOSED_DOOR)
		kick = MOVE_N;
	else if (saiph->branches[branch]->map[dungeon][row][col + 1] == CLOSED_DOOR)
		kick = MOVE_E;
	else if (saiph->branches[branch]->map[dungeon][row][col - 1] == CLOSED_DOOR)
		kick = MOVE_W;
	if (kick != -1) {
		/* what is this? a closed door right next to us?
		 * kick it! */
		char command[3];
		command[0] = 4;
		command[1] = kick;
		command[2] = '\0';
		saiph->setNextCommand(command, DO_KICK_DOOR);
	}
}
