#include "ExploreAnalyzer.h"

/* constructors */
ExploreAnalyzer::ExploreAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	this->type = ANALYZE_PASSABLE | ANALYZE_PLAYER;
	this->place_count = 0;
	this->max_score = 0;
}

/* destructors */
ExploreAnalyzer::~ExploreAnalyzer() {
}

/* methods */
void ExploreAnalyzer::analyze(int row, int col, char symbol) {
	if (place_count >= EA_MAX_PLACES)
		return; // tracking too many unexplored places
	int score = 0;
	for (int r = row - 1; r <= row + 1; ++r) {
		if (r < MAP_ROW_START || r >= MAP_ROW_END)
			continue;
		for (int c = col - 1; c <= col + 1; ++c) {
			if (c < 0 || c >= COLS)
				continue;
			if ((r == row || c == col) && saiph->isUnpassable(saiph->world->map[r][c]))
				score += MAX_SEARCH - saiph->branch[saiph->current_branch].search[r][c];
		}
	}
	if (score <= 0)
		return; // this place seems to be fully searched out
	cerr << "ExploreAnalyzer score: " << score << endl;
	/* simple attempt to make it search neaby places first
	 * this doesn't care if there's no quick path there */
	score += ROWS + COLS - abs(saiph->world->player.row - row) - abs(saiph->world->player.col - col);
	places[place_count].row = row;
	places[place_count].col = col;
	places[place_count].value = score;
	++place_count;
	max_score = score;
}

void ExploreAnalyzer::finish() {
	/* all the spots around the player are "semi searched" */
	for (int r = saiph->world->player.row - 1; r <= saiph->world->player.row + 1; ++r) {
		if (r < MAP_ROW_START || r >= MAP_ROW_END)
			continue;
		for (int c = saiph->world->player.col - 1; c <= saiph->world->player.col + 1; ++c) {
			if (c < 0 || c >= COLS)
				continue;
			if (r == saiph->world->player.row && c == saiph->world->player.col)
				saiph->branch[saiph->current_branch].search[r][c] = MAX_SEARCH;
			else if (saiph->isUnexplored(saiph->world->map[r][c]) && saiph->branch[saiph->current_branch].search[r][c] < MAX_SEARCH / 2)
				saiph->branch[saiph->current_branch].search[r][c] = MAX_SEARCH / 2;
		}
	}

	/* figure out which place to explore */
	for (int pa = 0; pa < place_count; ++pa) {
		int p = -1;
		int best = -1;
		for (int pb = 0; pb < place_count; ++pb) {
			if (places[pb].value > best) {
				p = pb;
				best = places[pb].value;
			}
		}
		if (p == -1)
			break; // found nowhere to go

		if (saiph->world->player.row == places[p].row && saiph->world->player.col == places[p].col) {
			/* no need to move, search instead */
			for (int r = places[p].row - 1; r <= places[p].row + 1; ++r) {
				if (r < MAP_ROW_START || r >= MAP_ROW_END)
					continue;
				for (int c = places[p].col - 1; c <= places[p].col + 1; ++c) {
					if (c < 0 || c >= COLS)
						continue;
					if (saiph->isUnexplored(saiph->world->map[r][c]))
						++saiph->branch[saiph->current_branch].search[r][c];
					else
						saiph->branch[saiph->current_branch].search[r][c] = MAX_SEARCH;
				}
			}
			char command[2];
			command[0] = 's';
			command[1] = '\0';
			saiph->setNextCommand(command, 40);
			break;
		} else {
			char move = saiph->shortestPath(places[p].row, places[p].col);
			if (move != -1) {
				char command[2];
				command[0] = move;
				command[1] = '\0';
				if (best > MAX_SEARCH / 2)
					saiph->setNextCommand(command, 40);
				else
					saiph->setNextCommand(command, 20);
				break;
			}
		}
		places[p].value = -1;
	}
	max_score = 0;
	place_count = 0;
}
