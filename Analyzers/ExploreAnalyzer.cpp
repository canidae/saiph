#include "ExploreAnalyzer.h"

/* constructors */
ExploreAnalyzer::ExploreAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	place_count = 0;
	symbols[symbol_count++] = FLOOR;
	symbols[symbol_count++] = CORRIDOR;
	symbols[symbol_count++] = OPEN_DOOR;
	symbols[symbol_count++] = CLOSED_DOOR;
	symbols[symbol_count++] = STAIRS_UP;
	symbols[symbol_count++] = STAIRS_DOWN;
	symbols[symbol_count++] = ALTAR;
	symbols[symbol_count++] = GRAVE;
	symbols[symbol_count++] = THRONE;
	symbols[symbol_count++] = SINK;
	symbols[symbol_count++] = FOUNTAIN;
	symbols[symbol_count++] = ICE;
	symbols[symbol_count++] = LOWERED_DRAWBRIDGE;
	symbols[symbol_count++] = WEAPON;
	symbols[symbol_count++] = ARMOR;
	symbols[symbol_count++] = RING;
	symbols[symbol_count++] = AMULET;
	symbols[symbol_count++] = TOOL;
	symbols[symbol_count++] = FOOD;
	symbols[symbol_count++] = POTION;
	symbols[symbol_count++] = SCROLL;
	symbols[symbol_count++] = SPELLBOOK;
	symbols[symbol_count++] = WAND;
	symbols[symbol_count++] = GOLD;
	symbols[symbol_count++] = GEM;
	symbols[symbol_count++] = STATUE;
	symbols[symbol_count++] = IRON_BALL;
	symbols[symbol_count++] = PLAYER;
}

/* methods */
void ExploreAnalyzer::analyze(int row, int col, char symbol) {
	if (row <= MAP_ROW_START || row >= MAP_ROW_END - 1 || col <= 0 || col >= COLS - 1 || place_count >= EX_MAX_PLACES)
		return;
	int branch = saiph->current_branch;
	int dungeon = saiph->world->player.status.dungeon;
	if (saiph->branches[branch]->search[dungeon][row][col] >= MAX_SEARCH)
		return; // we've been here and searched frantically
	/* hjkl symbols */
	char hs = saiph->branches[branch]->map[dungeon][row][col - 1];
	char js = saiph->branches[branch]->map[dungeon][row + 1][col];
	char ks = saiph->branches[branch]->map[dungeon][row - 1][col];
	char ls = saiph->branches[branch]->map[dungeon][row][col + 1];
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
	/* are we on floor? */
	bool floor = (saiph->branches[branch]->map[dungeon][row][col] == FLOOR);
	/* figure out score */
	int score = 0;
	bool dead_end = false;
	if (floor && (hune || june || kune || lune)) {
		score = EX_UNLIT_ROOM;
	} else if (!floor && ((h && j && k) || (j && k && l) || (h && k && l) || (h && j && l))) {
		score = EX_DEAD_END;
		dead_end = true;
	} else if (!floor && ((h && j) || (h && k) || (j && l) || (k && l))) {
		score = EX_TURN;
		/* hack to make it stop searching every turn when it passed the turn diagonally */
		bool hv = saiph->branches[branch]->search[dungeon][row][col - 1] >= MAX_SEARCH;
		bool jv = saiph->branches[branch]->search[dungeon][row + 1][col] >= MAX_SEARCH;
		bool kv = saiph->branches[branch]->search[dungeon][row - 1][col] >= MAX_SEARCH;
		bool lv = saiph->branches[branch]->search[dungeon][row][col + 1] >= MAX_SEARCH;
		if (((h && j && kv && lv) || (h && k && jv && lv) || (j && l && hv && kv) || (k && l && hv && jv))) {
			saiph->branches[branch]->search[dungeon][row][col] = MAX_SEARCH;
			return;
		}
	} else if (floor && (hunp || junp || kunp || lunp)) {
		score = EX_EXTRA_SEARCH;
	}

	if (symbol == PLAYER && saiph->branches[branch]->map[dungeon][row][col] == CORRIDOR && !dead_end) {
		/* don't search in corridors unless it's a dead end */
		saiph->branches[branch]->search[dungeon][row][col] = MAX_SEARCH;
		return;
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
		int nearest = ROWS * COLS;
		int best = 0;
		for (int pb = 0; pb < place_count; ++pb) {
			if (places[pb].value < 0)
				continue;
			int distance = 0;
			bool direct_line = false;
			if (places[pb].value >= best) {
				places[pb].move = saiph->shortestPath(places[pb].row, places[pb].col, false, distance, direct_line);
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
			++saiph->branches[branch]->search[dungeon][places[p].row][places[p].col];
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
