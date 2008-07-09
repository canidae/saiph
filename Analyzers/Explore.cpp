#include "Explore.h"

/* constructors */
Explore::Explore(Saiph *saiph) {
	this->saiph = saiph;
	move = ILLEGAL_MOVE;
	vector<unsigned char> symbols;
	explore_priority[0] = EXPLORE_PRIORITY0;
	explore_priority[1] = EXPLORE_PRIORITY1;
	explore_priority[2] = EXPLORE_PRIORITY2;
	symbols.push_back(FLOOR);
	symbols.push_back(CORRIDOR);
	symbols.push_back(OPEN_DOOR);
	symbols.push_back(PLAYER);
	saiph->registerAnalyzerSymbols(this, symbols);
}

/* methods */
void Explore::analyze(int row, int col, unsigned char symbol) {
	if (row <= MAP_ROW_BEGIN || row >= MAP_ROW_END || col <= MAP_COL_BEGIN || col >= MAP_COL_END)
		return;
	int b = saiph->current_branch;
	int l = saiph->current_level;
	if (symbol == PLAYER) {
		/* any square we step on is fully explored */
		search[b][l][row][col] = EXPLORE_SEARCH_COUNT;
		return;
	}
	/* what does the squares around this square look like? */
	unsigned char hs = saiph->map[b][l].dungeon[row][col - 1];
	unsigned char js = saiph->map[b][l].dungeon[row + 1][col];
	unsigned char ks = saiph->map[b][l].dungeon[row - 1][col];
	unsigned char ls = saiph->map[b][l].dungeon[row][col + 1];
	/* have we searched the nearby squares before? */
	int hse = search[b][l][row][col - 1];
	int jse = search[b][l][row + 1][col];
	int kse = search[b][l][row - 1][col];
	int lse = search[b][l][row][col + 1];
	int yse = search[b][l][row - 1][col - 1];
	int use = search[b][l][row + 1][col + 1];
	int bse = search[b][l][row + 1][col - 1];
	int nse = search[b][l][row + 1][col + 1];
	int searched = hse + jse + kse + lse + yse + use + bse + nse;
	if (searched >= EXPLORE_SEARCH_COUNT * 8)
		return; // searched everything here before
	Point p;
	p.row = row;
	p.col = col;
	int count = 0;
	switch (symbol) {
		case CORRIDOR:
			if (hs == SOLID_ROCK || hs == HORIZONTAL_WALL || hs == VERTICAL_WALL)
				++count;
			if (js == SOLID_ROCK || js == HORIZONTAL_WALL || js == VERTICAL_WALL)
				++count;
			if (ks == SOLID_ROCK || ks == HORIZONTAL_WALL || ks == VERTICAL_WALL)
				++count;
			if (ls == SOLID_ROCK || ls == HORIZONTAL_WALL || ls == VERTICAL_WALL)
				++count;
			if (count == 3)
				explore[0].push_back(p); // dead end
			else if (count == 2 && !((hs != SOLID_ROCK && ls != SOLID_ROCK) || (js != SOLID_ROCK && ks != SOLID_ROCK)))
				explore[1].push_back(p); // corridor turning
			break;

		case FLOOR:
			if (hs == SOLID_ROCK || js == SOLID_ROCK || ks == SOLID_ROCK || ls == SOLID_ROCK)
				explore[0].push_back(p); // unexplored square next to floor
			else if ((hs == VERTICAL_WALL || ls == VERTICAL_WALL) && (js == HORIZONTAL_WALL || ks == HORIZONTAL_WALL))
				explore[1].push_back(p); // corner of a room
			else if (search[b][l][row][col] < EXPLORE_SEARCH_COUNT && (hs == VERTICAL_WALL || js == HORIZONTAL_WALL || ks == HORIZONTAL_WALL || ls == VERTICAL_WALL))
				explore[2].push_back(p); // next to a wall in a room
			break;

		case OPEN_DOOR:
			if (hs == SOLID_ROCK || js == SOLID_ROCK || ks == SOLID_ROCK || ls == SOLID_ROCK)
				explore[0].push_back(p); // at least one of the sides of this door is unexplored
			break;

		default:
			return;
	}
}

void Explore::command(string *command) {
	if (move == SEARCH) {
		for (int r = saiph->world->player.row - 1; r <= saiph->world->player.row + 1; ++r) {
			for (int c = saiph->world->player.col - 1; c <= saiph->world->player.col + 1; ++c) {
				if (r == saiph->world->player.row && c == saiph->world->player.col)
					continue;
				++search[saiph->current_branch][saiph->current_level][r][c];
			}
		}
	}
	command->push_back(move);
}

int Explore::finish() {
	/* figure out which place to explore */
	int shortest_distance = INT_MAX;
	move = ILLEGAL_MOVE;
	for (int e = 0; e < EXPLORE_PRIORITIES; ++e) {
		for (vector<Point>::iterator p = explore[e].begin(); p != explore[e].end(); ++p) {
			int distance = 0;
			bool straight_line = false;
			unsigned char nextmove = saiph->shortestPath(*p, false, &distance, &straight_line);
			if (nextmove == ILLEGAL_MOVE)
				continue;
			if (nextmove == REST) {
				move = SEARCH;
				return explore_priority[e];
			}
			if (distance < shortest_distance) {
				shortest_distance = distance;
				move = nextmove;
			}
		}
		if (shortest_distance < INT_MAX)
			return explore_priority[e];
	}
	return 0;
}

int Explore::start() {
	for (int e = 0; e < EXPLORE_PRIORITIES; ++e)
		explore[e].clear();
	return 0;
}
