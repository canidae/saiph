#include "Level.h"

/* constructors */
Level::Level(Saiph *saiph) : Analyzer("Level") {
	this->saiph = saiph;
}

/* methods */
void Level::complete() {
}

void Level::finish() {
	/* time to descend */
	command = "";
	if (saiph->dungeonmap[saiph->position.branch][saiph->position.level][saiph->world->player.row][saiph->world->player.col] == STAIRS_DOWN) {
		/* standing on downstairs, descend */
		command = MOVE_DOWN;
		priority = LEVEL_DESCEND_PRIORITY;
		return;
	}
	for (vector<Coordinate>::iterator s = stairs.begin(); s != stairs.end(); ++s) {
		if (s->branch != saiph->position.branch || s->level != saiph->position.level)
			continue;
		if (saiph->dungeonmap[s->branch][s->level][s->row][s->col] == STAIRS_DOWN) {
			int distance = 0;
			unsigned char move = saiph->shortestPath(*s, false, &distance);
			if (move != ILLEGAL_MOVE) {
				command = move;
				priority = LEVEL_DESCEND_PRIORITY;
				return;
			}
		}
	}
}

void Level::inspect(const Point &point) {
	/* find stairs */
	unsigned char symbol = saiph->dungeonmap[saiph->position.branch][saiph->position.level][point.row][point.col];
	if (symbol != STAIRS_DOWN && symbol != STAIRS_UP)
		return; // not stairs
	/* figure out if we already know about these stairs */
	for (vector<Coordinate>::iterator s = stairs.begin(); s != stairs.end(); ++s) {
		if (s->branch != saiph->position.branch || s->level != saiph->position.level)
			continue;
		if (s->row == point.row && s->col == point.col)
			return; // already know of these stairs
	}
	/* add stairs to list */
	stairs.push_back(Coordinate(saiph->position.branch, saiph->position.level, point));
}
