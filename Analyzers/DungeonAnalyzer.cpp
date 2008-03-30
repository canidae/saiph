#include "DungeonAnalyzer.h"

/* constructors */
DungeonAnalyzer::DungeonAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	this->type = ANALYZE_ALL;
}

/* destructors */
DungeonAnalyzer::~DungeonAnalyzer() {
}

/* methods */
void DungeonAnalyzer::analyze(int row, int col, char symbol) {
	if (saiph->isOpenDoor(symbol)) {
		saiph->branches[saiph->current_branch].map[saiph->world->player.status.dungeon][row][col] = OPEN_DOOR;
	} else if (saiph->isPassable(symbol) || saiph->isUnpassable(symbol)) {
		saiph->branches[saiph->current_branch].map[saiph->world->player.status.dungeon][row][col] = symbol;
	}
}

void DungeonAnalyzer::finish() {
}
