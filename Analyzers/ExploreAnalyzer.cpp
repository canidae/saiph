#include "ExploreAnalyzer.h"

/* constructors */
ExploreAnalyzer::ExploreAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	place_count = 0;
	symbols[symbol_count++] = FLOOR;
	symbols[symbol_count++] = OPEN_DOOR;
	symbols[symbol_count++] = CORRIDOR;
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
	if (saiph->branches[branch].search[dungeon][row][col] >= MAX_SEARCH)
		return; // we've been here and searched frantically
	/* hjkl symbols */
	char hs = saiph->branches[branch].map[dungeon][row][col - 1];
	char js = saiph->branches[branch].map[dungeon][row + 1][col];
	char ks = saiph->branches[branch].map[dungeon][row - 1][col];
	char ls = saiph->branches[branch].map[dungeon][row][col + 1];
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
	/* are we on floor? */
	bool floor = (saiph->branches[branch].map[dungeon][row][col] == FLOOR);
	/* figure out score */
	int score = 0;
	if ((hune && june && kune) || (june && kune && lune) || (hune && kune && lune) || (hune && june && lune))
		score = EX_DEAD_END;
	else if ((hune && june) || (hune && kune) || (june && lune) || (kune && lune))
		score = floor ? EX_UNLIT_ROOM : EX_TURN;
	else if ((hunp && lunp && (june || kune)) || (junp && kunp && (hune || lune)))
		score = EX_ROOM_EXIT;
	else if ((hunp || junp || kunp || lunp) && floor)
		score = EX_EXTRA_SEARCH;

	if (score <= 0)
		return; // this place isn't very exciting
	cerr << "ExploreAnalyzer " << row << ", " << col << ": " << score << endl;
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
				places[pb].move = saiph->shortestPath(places[pb].row, places[pb].col, distance, direct_line);
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
