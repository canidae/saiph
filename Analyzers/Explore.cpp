#include "Explore.h"

/* constructors */
Explore::Explore(Saiph *saiph) {
	this->saiph = saiph;
	memset(search, 0, sizeof (search));
	move = ILLEGAL_MOVE;
	vector<unsigned char> symbols;
	symbols.push_back(CORRIDOR);
	symbols.push_back(FLOOR);
	symbols.push_back(OPEN_DOOR);
	symbols.push_back(PLAYER);
	saiph->registerAnalyzerSymbols(this, symbols);
}

/* methods */
void Explore::command(string *command) {
	if (move == SEARCH)
		++search[saiph->current_branch][saiph->current_level][saiph->world->row][saiph->world->col];
	command->push_back(move);
}

int Explore::finish() {
	/* figure out which place to explore */
	move = ILLEGAL_MOVE;
	for (list<ExplorePoint>::iterator e = explore.begin(); e != explore.end(); ++e) {
		int distance = 0;
		bool straight_line = false;
		unsigned char nextmove = saiph->shortestPath(e->loc, false, &distance, &straight_line);
		if (nextmove == ILLEGAL_MOVE)
			continue;
		if (nextmove == REST) {
			move = SEARCH;
		} else {
			move = nextmove;
			return e->priority;
		}
	}
	return 0;
}

void Explore::inspect(int row, int col, unsigned char symbol) {
	int b = saiph->current_branch;
	int l = saiph->current_level;
	if (search[b][l][row][col] >= EXPLORE_SEARCH_COUNT)
		return; // been here before
	if (symbol == CORRIDOR) {
		ExplorePoint ep;
		ep.loc.row = row;
		ep.loc.col = col;
		ep.priority = EXPLORE_VISIT_CORRIDOR;
		addExplorePoint(ep);
		return;
	} else if (symbol == OPEN_DOOR) {
		ExplorePoint ep;
		ep.loc.row = row;
		ep.loc.col = col;
		ep.priority = EXPLORE_VISIT_OPEN_DOOR;
		addExplorePoint(ep);
		return;
	} else if (symbol == FLOOR) {
		/* we only want to visit floor tiles where one of the adjacent squares is SOLID_ROCK or a wall */
		unsigned char hs = saiph->map[b][l].dungeon[row][col - 1];
		unsigned char js = saiph->map[b][l].dungeon[row + 1][col];
		unsigned char ks = saiph->map[b][l].dungeon[row - 1][col];
		unsigned char ls = saiph->map[b][l].dungeon[row][col + 1];
		if (hs == SOLID_ROCK || hs == VERTICAL_WALL || js == SOLID_ROCK || js == HORIZONTAL_WALL || ks == SOLID_ROCK || ks == HORIZONTAL_WALL || ls == SOLID_ROCK || ls == VERTICAL_WALL) {
		}
	} else if (symbol == PLAYER) {
		/* any square we step on is fully explored */
		search[b][l][row][col] = EXPLORE_SEARCH_COUNT;
		return;
	}
	/* what does the squares around this square look like? */
	/*
	unsigned char hs = saiph->map[b][l].dungeon[row][col - 1];
	unsigned char js = saiph->map[b][l].dungeon[row + 1][col];
	unsigned char ks = saiph->map[b][l].dungeon[row - 1][col];
	unsigned char ls = saiph->map[b][l].dungeon[row][col + 1];
	*/
	/* have we searched the nearby squares before? */
	/*
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
	*/
}

/* private methods */
void Explore::addExplorePoint(const ExplorePoint &ep) {
	list<ExplorePoint>::iterator e;
	for (e = explore.begin(); e != explore.end(); ++e) {
		if (e->loc.row == ep.loc.row && e->loc.col == ep.loc.col)
			return; // this place already is in the list with higher priority
		if (e->priority < ep.priority)
			break;
	}
	explore.insert(e, ep);
}
