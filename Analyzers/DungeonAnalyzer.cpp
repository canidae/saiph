#include "DungeonAnalyzer.h"

/* constructors */
DungeonAnalyzer::DungeonAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
}

/* methods */
void DungeonAnalyzer::start() {
	/* certain analyzers depend on the maps being updated,
	 * for example, ExploreAnalyzer.
	 * sadly that means we'll have to make this one go through
	 * all tiles now */
	int branch = saiph->current_branch;
	int dungeon = saiph->world->player.dungeon;

	for (int r = MAP_ROW_START; r <= MAP_ROW_END; ++r) {
		for (int c = 0; c < COLS; ++c) {
			char symbol = saiph->world->map[r][c];
			if (symbol == SOLID_ROCK) {
				/* nothing exciting, carry on */
			} else if (symbol == VERTICAL_WALL || symbol == HORIZONTAL_WALL ||symbol == FLOOR || symbol == OPEN_DOOR || symbol == CLOSED_DOOR || symbol == IRON_BARS || symbol == TREE || symbol == CORRIDOR || symbol == STAIRS_UP || symbol == STAIRS_DOWN || symbol == ALTAR || symbol == GRAVE || symbol == THRONE || symbol == SINK || symbol == FOUNTAIN || symbol == WATER || symbol == ICE || symbol == LAVA || symbol == LOWERED_DRAWBRIDGE || symbol == RAISED_DRAWBRIDGE || symbol == TRAP) {
				/* "static" dungeon features (doors may be destroyed, though).
				 * update the map showing static stuff */
				saiph->branches[branch]->map[dungeon][r][c] = symbol;
			} else if (saiph->branches[branch]->map[dungeon][r][c] == SOLID_ROCK) {
				/* "dynamic" stuff that can disappear on a spot we've never seen before.
				 * pretend there's an open door here until we see otherwise */
				saiph->branches[branch]->map[dungeon][r][c] = OPEN_DOOR;
			} else if (saiph->branches[branch]->map[dungeon][r][c] == CLOSED_DOOR) {
				/* there used to be a door here, but now something else is here.
				 * it's quite possible a monster opened the door */
				saiph->branches[branch]->map[dungeon][r][c] = OPEN_DOOR;
			} else if (saiph->branches[branch]->map[dungeon][r][c] == HORIZONTAL_WALL || saiph->branches[branch]->map[dungeon][r][c] == VERTICAL_WALL) {
				/* there used to be a wall here, but isn't no longer.
				 * make it an open door for the time being */
				saiph->branches[branch]->map[dungeon][r][c] = OPEN_DOOR;
			}
		}
	}
}
