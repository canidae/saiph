#include "Level.h"

/* constructors */
Level::Level(Saiph *saiph) : Analyzer("Level") {
	this->saiph = saiph;
	action = "";
}

/* methods */
void Level::command(string *command) {
	*command = action;
}

void Level::finish() {
	/* time to descend */
	action = "";
	if (saiph->map[saiph->position.branch][saiph->position.level].dungeon[saiph->world->player.row][saiph->world->player.col] == STAIRS_DOWN) {
		/* standing on downstairs, descend */
		action = MOVE_DOWN;
		priority = LEVEL_DESCEND_PRIORITY;
		return;
	}
	for (vector<Coordinate>::iterator s = stairs.begin(); s != stairs.end(); ++s) {
		if (s->branch != saiph->position.branch || s->level != saiph->position.level)
			continue;
		if (saiph->map[s->branch][s->level].dungeon[s->row][s->col] == STAIRS_DOWN) {
			int distance = 0;
			bool direct_line = false;
			unsigned char move = saiph->shortestPath(*s, false, &distance, &direct_line);
			if (move != ILLEGAL_MOVE) {
				action = move;
				priority = LEVEL_DESCEND_PRIORITY;
				return;
			}
		}
	}
}

void Level::inspect(const Point &point) {
	/* find stairs */
	unsigned char symbol = saiph->map[saiph->position.branch][saiph->position.level].dungeon[point.row][point.col];
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
