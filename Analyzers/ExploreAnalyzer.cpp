#include "ExploreAnalyzer.h"

/* constructors */
ExploreAnalyzer::ExploreAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	place_count = 0;
	best_place = -1;
	vector<unsigned char> symbols;
	symbols.push_back(FLOOR);
	symbols.push_back(CORRIDOR);
	symbols.push_back(OPEN_DOOR);
	symbols.push_back(STAIRS_UP);
	symbols.push_back(STAIRS_DOWN);
	symbols.push_back(ALTAR);
	symbols.push_back(GRAVE);
	symbols.push_back(THRONE);
	symbols.push_back(SINK);
	symbols.push_back(FOUNTAIN);
	symbols.push_back(ICE);
	symbols.push_back(LOWERED_DRAWBRIDGE);
	symbols.push_back(WEAPON);
	symbols.push_back(ARMOR);
	symbols.push_back(RING);
	symbols.push_back(AMULET);
	symbols.push_back(TOOL);
	symbols.push_back(FOOD);
	symbols.push_back(POTION);
	symbols.push_back(SCROLL);
	symbols.push_back(SPELLBOOK);
	symbols.push_back(WAND);
	symbols.push_back(GOLD);
	symbols.push_back(GEM);
	symbols.push_back(STATUE);
	symbols.push_back(IRON_BALL);
	symbols.push_back(PLAYER);
	saiph->registerAnalyzerSymbols(this, symbols);
}

/* methods */
void ExploreAnalyzer::analyze(int row, int col, unsigned char symbol) {
	if (row <= MAP_ROW_BEGIN || row >= MAP_ROW_END || col <= MAP_COL_BEGIN || col >= MAP_COL_END || place_count >= EX_MAX_PLACES)
		return;
	int branch = saiph->current_branch;
	int dungeon = saiph->world->player.dungeon;
	/* are we in a corridor? */
	bool corridor = saiph->map[branch][dungeon].dungeon[row][col] == CORRIDOR;
	if ((!corridor && saiph->map[branch][dungeon].search[row][col] >= EX_MAX_SEARCH) || (corridor && saiph->map[branch][dungeon].search[row][col] >= EX_MAX_SEARCH * EX_DEAD_END_MULTIPLIER))
		return; // we've been here and searched frantically
	/* hjkl symbols */
	unsigned char hs = saiph->map[branch][dungeon].dungeon[row][col - 1];
	unsigned char js = saiph->map[branch][dungeon].dungeon[row + 1][col];
	unsigned char ks = saiph->map[branch][dungeon].dungeon[row - 1][col];
	unsigned char ls = saiph->map[branch][dungeon].dungeon[row][col + 1];
	/* hjkl unexplored */
	bool hune = (hs == SOLID_ROCK);
	bool june = (js == SOLID_ROCK);
	bool kune = (ks == SOLID_ROCK);
	bool lune = (ls == SOLID_ROCK);
	/* hjkl unpassable (only walls) */
	bool hunp = (hs == VERTICAL_WALL || hs == HORIZONTAL_WALL);
	bool junp = (js == VERTICAL_WALL || js == HORIZONTAL_WALL);
	bool kunp = (ks == VERTICAL_WALL || ks == HORIZONTAL_WALL);
	bool lunp = (ls == VERTICAL_WALL || ls == HORIZONTAL_WALL);
	/* *une || *unp */
	bool h = hune || hunp;
	bool j = june || junp;
	bool k = kune || kunp;
	bool l = lune || lunp;
	/* figure out score */
	int score = 0;
	bool dead_end = false;
	if (!corridor && (hune || june || kune || lune)) {
		score = EX_UNLIT_ROOM;
	} else if (corridor) {
		score = EX_CORRIDOR;
		if ((h && j && k) || (j && k && l) || (h && k && l) || (h && j && l))
			dead_end = true;
	} else if (!corridor && (hunp || junp || kunp || lunp)) {
		score = EX_EXTRA_SEARCH;
	}

	if (symbol == PLAYER && saiph->map[branch][dungeon].dungeon[row][col] == CORRIDOR && !dead_end) {
		/* don't search in corridors unless it's a dead end */
		saiph->map[branch][dungeon].search[row][col] = EX_MAX_SEARCH * EX_DEAD_END_MULTIPLIER;
		return;
	}
	if (score <= 0)
		return; // this place isn't very exciting
	places[place_count].row = row;
	places[place_count].col = col;
	places[place_count].move = ILLEGAL_MOVE;
	places[place_count].value = score;
	++place_count;
	return;
}

int ExploreAnalyzer::finish() {
	/* figure out which place to explore */
	int best = 0;
	int shortest_distance = -1;
	best_place = -1;
	for (int p = 0; p < place_count; ++p) {
		if (places[p].value < best && shortest_distance != -1)
			continue;
		int distance = 0;
		bool direct_line = false;
		Point tmp;
		tmp.row = places[p].row;
		tmp.col = places[p].col;
		places[p].move = saiph->shortestPath(tmp, false, &distance, &direct_line);
		if (places[p].move == ILLEGAL_MOVE)
			continue;
		if (places[p].value > best || (places[p].value == best && (shortest_distance == -1 || distance < shortest_distance))) {
			shortest_distance = distance;
			best = places[p].value;
			best_place = p;
		}
	}
	place_count = 0;

	if (best_place == -1)
		return best;

	if (places[best_place].move == REST)
		places[best_place].move = SEARCH; // search instead of rest
	return best;
}

void ExploreAnalyzer::command(string *command) {
	if (places[best_place].move == SEARCH)
		++saiph->map[saiph->current_branch][saiph->current_level].search[places[best_place].row][places[best_place].col];
	command->push_back(places[best_place].move);
}
