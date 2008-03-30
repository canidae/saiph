#include "DungeonAnalyzer.h"

/* constructors */
DungeonAnalyzer::DungeonAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	this->type = ANALYZE_NONE;
}

/* destructors */
DungeonAnalyzer::~DungeonAnalyzer() {
}

/* methods */
void DungeonAnalyzer::start() {
	/* certain analyzers depend on the maps being updated,
	 * for example, ExploreAnalyzer.
	 * sadly that means we'll have to make this one go through
	 * all tiles now */

	int branch = saiph->current_branch;
	int dungeon = saiph->world->player.status.dungeon;

	for (int r = MAP_ROW_START; r <= MAP_ROW_END; ++r) {
		for (int c = 0; c < COLS; ++c) {
			char symbol = saiph->world->map[r][c];
			if (saiph->isClosedDoor(symbol) || saiph->isOpenDoor(symbol) || saiph->isPassable(symbol) || saiph->isUnexplored(symbol) || saiph->isUnpassable(symbol) || saiph->isBoulder(symbol)) {
				/* "static" dungeon features (doors may be destroyed, though).
				 * update the map showing static stuff */
				saiph->branches[branch].map[dungeon][r][c] = symbol;
			} else if (saiph->hasUnexplored(branch, dungeon, r, c)) {
				/* "dynamic" stuff that can disappear on a spot we've never seen before.
				 * pretend there's an open door here until we see otherwise */
				saiph->branches[branch].map[dungeon][r][c] = OPEN_DOOR;
			} else if (saiph->hasClosedDoor(branch, dungeon, r, c)) {
				/* there used to be a door here, but now something else is here.
				 * it's quite possible a monster opened the door */
				saiph->branches[branch].map[dungeon][r][c] = OPEN_DOOR;
			} else if (saiph->hasUnpassable(branch, dungeon, r, c)) {
				/* a wall is gone? */
				saiph->branches[branch].map[dungeon][r][c] = OPEN_DOOR;
			}
		}
	}
}
