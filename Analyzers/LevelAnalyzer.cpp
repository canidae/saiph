#include "LevelAnalyzer.h"

/* constructors */
LevelAnalyzer::LevelAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	this->type = ANALYZE_STAIR;
}

/* destructors */
LevelAnalyzer::~LevelAnalyzer() {
}

/* methods */
void LevelAnalyzer::end() {
	if (saiph->command.priority > LA_DESCEND_PRIORITY)
		return;
	/* time to descend */
	int branch = saiph->current_branch;
	int dungeon = saiph->world->player.status.dungeon;
	if (saiph->hasDownStairs(branch, dungeon, saiph->world->player.row, saiph->world->player.col)) {
		/* standing on downstairs, descend */
		char command[2];
		command[0] = MOVE_DOWN;
		command[1] = '\0';
		saiph->setNextCommand(command, LA_DESCEND_PRIORITY);
		return;
	}
	for (int r = MAP_ROW_START; r <= MAP_ROW_END; ++r) {
		for (int c = 0; c < COLS; ++c) {
			if (saiph->hasDownStairs(branch, dungeon, r, c)) {
				int distance = 0;
				bool direct_line = false;
				char move = saiph->shortestPath(branch, dungeon, r, c, distance, direct_line);
				if (move != -1) {
					char command[2];
					command[0] = move;
					command[1] = '\0';
					saiph->setNextCommand(command, LA_DESCEND_PRIORITY);
					return;
				}
			}
		}
	}
}
