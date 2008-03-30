#include "ExploreAnalyzer.h"

/* constructors */
ExploreAnalyzer::ExploreAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	this->type = ANALYZE_PASSABLE | ANALYZE_PLAYER | ANALYZE_OBJECT | ANALYZE_OPEN_DOOR;
	this->place_count = 0;
}

/* destructors */
ExploreAnalyzer::~ExploreAnalyzer() {
}

/* methods */
void ExploreAnalyzer::analyze(int row, int col, char symbol) {
	if (row <= MAP_ROW_START || row >= MAP_ROW_END - 1 || col <= 0 || col >= COLS - 1 || place_count >= EA_MAX_PLACES)
		return;
	int branch = saiph->current_branch;
	int dungeon = saiph->world->player.status.dungeon;
	if (saiph->branches[branch].search[dungeon][row][col] >= MAX_SEARCH)
		return; // we've been here and searched frantically
	int score = 0;
	bool hune = saiph->hasUnexplored(branch, dungeon, row, col - 1);
	bool june = saiph->hasUnexplored(branch, dungeon, row + 1, col);
	bool kune = saiph->hasUnexplored(branch, dungeon, row - 1, col);
	bool lune = saiph->hasUnexplored(branch, dungeon, row, col + 1);
	int une = (hune ? 1 : 0) + (june ? 1 : 0) + (kune ? 1 : 0) + (lune ? 1 : 0);
	bool hunp = saiph->hasUnpassable(branch, dungeon, row, col - 1);
	bool junp = saiph->hasUnpassable(branch, dungeon, row + 1, col);
	bool kunp = saiph->hasUnpassable(branch, dungeon, row - 1, col);
	bool lunp = saiph->hasUnpassable(branch, dungeon, row, col + 1);
	int unp = (hunp ? 1 : 0) + (junp ? 1 : 0) + (kunp ? 1 : 0) + (lunp ? 1 : 0);
	bool h = hune || hunp;
	bool j = june || junp;
	bool k = kune || kunp;
	bool l = lune || lunp;
	bool hhc = saiph->hasCorridor(branch, dungeon, row, col - 1);
	bool jhc = saiph->hasCorridor(branch, dungeon, row + 1, col);
	bool khc = saiph->hasCorridor(branch, dungeon, row - 1, col);
	bool lhc = saiph->hasCorridor(branch, dungeon, row, col + 1);
	if ((h && j && k) || (j && k && l) || (h && k && l) || (h && j && l)) {
		/* dead end.
		 * this is a really interesting place */
		if (une > unp)
			score = EA_DEAD_END_VALUE;
		else
			score = (EA_DEAD_END_VALUE + EA_TURN_VALUE) / 2;
	} else if ((h && j) || (h && k) || (j && l) || (k && l)) {
		/* turn.
		 * turns in rooms aren't that interesting. */
		int corridors = hhc ? 1 : 0;
		corridors += jhc ? 1 : 0;
		corridors += khc ? 1 : 0;
		corridors += lhc ? 1 : 0;
		if (corridors > 0 && une > unp)
			score = EA_TURN_VALUE;
		else if (corridors > 0)
			score = (EA_TURN_VALUE + EA_STRAIGHT_PATH_VALUE) / 2;
		else if (une > unp)
			score = EA_STRAIGHT_PATH_VALUE;
		else
			score = (EA_STRAIGHT_PATH_VALUE + EA_T_CROSS_VALUE) / 2;
	} else if ((h && l && !jhc && !khc) || (j && k && !hhc && !lhc)) {
		/* straight path, not in a corridor */
		if (une > unp)
			score = EA_STRAIGHT_PATH_VALUE;
		else
			score = (EA_STRAIGHT_PATH_VALUE + EA_T_CROSS_VALUE) / 2;
	} else if ((h || j || k || l) && !(lhc || jhc || khc || lhc)) {
		/* T-cross, not in a corridor.
		 * this is really desperate searching.
		 * should only be done when no stairs are found */
		if (une > unp)
			score = EA_T_CROSS_VALUE;
		else
			score = (EA_T_CROSS_VALUE + EA_NOTHING_VALUE) / 2;
	}
	if (score <= 0)
		return; // this place isn't very exciting
	places[place_count].row = row;
	places[place_count].col = col;
	places[place_count].move = -1;
	places[place_count].value = score;
	++place_count;
}

void ExploreAnalyzer::finish() {
	int branch = saiph->current_branch;
	int dungeon = saiph->world->player.status.dungeon;
	/* figure out which place to explore */
	for (int pa = 0; pa < place_count; ++pa) {
		int p = -1;
		int nearest = MAX_NODES;
		int best = 0;
		for (int pb = 0; pb < place_count; ++pb) {
			if (places[pb].value < 0)
				continue;
			int distance = 0;
			bool direct_line = false;
			if (places[pb].value >= best) {
				places[pb].move = saiph->shortestPath(branch, dungeon, places[pb].row, places[pb].col, distance, direct_line);
				if (places[pb].value > best || distance < nearest) {
					p = pb;
					nearest = distance;
					best = places[pb].value;
				}
			}
		}
		if (p == -1)
			break; // found nowhere to go

		if (saiph->world->player.row == places[p].row && saiph->world->player.col == places[p].col) {
			/* no need to move, search instead */
			++saiph->branches[branch].search[dungeon][places[p].row][places[p].col];
			char command[2];
			command[0] = 's';
			command[1] = '\0';
			saiph->setNextCommand(command, best);
			break;
		} else if (places[p].move != -1) {
			char command[2];
			command[0] = places[p].move;
			command[1] = '\0';
			saiph->setNextCommand(command, best);
			break;
		}
		places[p].value = -1;
	}
	place_count = 0;
}
