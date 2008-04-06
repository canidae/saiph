#include "LevelAnalyzer.h"

/* constructors */
LevelAnalyzer::LevelAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	action = -1;
}

/* methods */
int LevelAnalyzer::finish() {
	if (saiph->command.priority > LA_DESCEND_PRIORITY)
		return 0;
	/* time to descend */
	int branch = saiph->current_branch;
	int dungeon = saiph->world->player.dungeon;
	if (saiph->branches[branch]->map[dungeon][saiph->world->player.row][saiph->world->player.col] == STAIRS_DOWN) {
		/* standing on downstairs, descend */
		return LA_DESCEND_PRIORITY;
	}
	for (int r = MAP_ROW_START; r <= MAP_ROW_END; ++r) {
		for (int c = 0; c < COLS; ++c) {
			if (saiph->branches[branch]->map[dungeon][r][c] == STAIRS_DOWN) {
				int distance = 0;
				bool direct_line = false;
				char move = saiph->shortestPath(r, c, false, distance, direct_line);
				if (move != -1) {
					action = move;
					return LA_DESCEND_PRIORITY;
				}
			}
		}
	}
	return 0;
}

void LevelAnalyzer::command() {
	char command[2];
	command[0] = action;
	command[1] = '\0';
	saiph->world->command(command);
}
