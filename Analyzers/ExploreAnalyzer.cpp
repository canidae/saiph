#include "ExploreAnalyzer.h"

/* constructors */
ExploreAnalyzer::ExploreAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	this->type = ANALYZE_PASSABLE;
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
			score += (MAX_SEARCH - saiph->branches[saiph->current_branch].search[saiph->current_branch][r][c]);
		}
	}
	if (score <= 0)
		return; // this place seems to be fully searched out
	if (abs(saiph->world->player.row - row) <= 1 && abs(saiph->world->player.col - col) <= 1)
		score *= 2; // make it prefer searching nearby squares
	cerr << "ExploreAnalyzer score (" << row << ", " << col << "): " << score << endl;
	places[place_count].row = row;
	places[place_count].col = col;
	places[place_count].value = score;
	++place_count;
	max_score = score;
}

void ExploreAnalyzer::finish() {
	/* set all the passable spots around the player to "fully searched" */
	for (int r = saiph->world->player.row - 1; r <= saiph->world->player.row + 1; ++r) {
		if (r < MAP_ROW_START || r >= MAP_ROW_END)
			continue;
		for (int c = saiph->world->player.col - 1; c <= saiph->world->player.col + 1; ++c) {
			if (c < 0 || c >= COLS)
				continue;
			if ((r == saiph->world->player.row && c == saiph->world->player.col) || saiph->isPassable(saiph->world->map[r][c]))
				saiph->branches[saiph->current_branch].search[saiph->current_branch][r][c] = MAX_SEARCH;
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

		cerr << "Trying to go to " << places[p].row << ", " << places[p].col << ": '" << saiph->world->map[places[p].row][places[p].col] << "': " << best << endl;
		if (saiph->world->player.row == places[p].row && saiph->world->player.col == places[p].col) {
			/* no need to move, search instead */
			for (int r = places[p].row - 1; r <= places[p].row + 1; ++r) {
				if (r < MAP_ROW_START || r >= MAP_ROW_END)
					continue;
				for (int c = places[p].col - 1; c <= places[p].col + 1; ++c) {
					if (c < 0 || c >= COLS)
						continue;
					if (saiph->isUnpassable(saiph->world->map[r][c]))
						++saiph->branches[saiph->current_branch].search[saiph->current_branch][r][c];
				}
			}
			char command[2];
			command[0] = 's';
			command[1] = '\0';
			saiph->setNextCommand(command, 40);
			break;
		} else {
			int distance = 0;
			bool direct_line = false;
			char move = saiph->shortestPath(saiph->current_branch, saiph->world->player.status.dungeon, places[p].row, places[p].col, distance, direct_line);
			cerr << "Move: " << move << endl;
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
