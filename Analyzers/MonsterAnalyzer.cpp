#include "MonsterAnalyzer.h"

/* constructors */
MonsterAnalyzer::MonsterAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	for (int m = 0; m < MO_MAX_MONSTERS; ++m) {
		monsters[m].row = -1;
		monsters[m].col = -1;
		monsters[m].symbol = NOMONSTER;
		monsters[m].last_seen = -1;
	}
	action = ILLEGAL_MOVE;
	symbols[symbol_count++] = 'a';
	symbols[symbol_count++] = 'b';
	symbols[symbol_count++] = 'c';
	symbols[symbol_count++] = 'd';
	symbols[symbol_count++] = 'e';
	symbols[symbol_count++] = 'f';
	symbols[symbol_count++] = 'g';
	symbols[symbol_count++] = 'h';
	symbols[symbol_count++] = 'i';
	symbols[symbol_count++] = 'j';
	symbols[symbol_count++] = 'k';
	symbols[symbol_count++] = 'l';
	symbols[symbol_count++] = 'm';
	symbols[symbol_count++] = 'n';
	symbols[symbol_count++] = 'o';
	symbols[symbol_count++] = 'p';
	symbols[symbol_count++] = 'q';
	symbols[symbol_count++] = 'r';
	symbols[symbol_count++] = 's';
	symbols[symbol_count++] = 't';
	symbols[symbol_count++] = 'u';
	symbols[symbol_count++] = 'v';
	symbols[symbol_count++] = 'w';
	symbols[symbol_count++] = 'x';
	symbols[symbol_count++] = 'y';
	symbols[symbol_count++] = 'z';
	symbols[symbol_count++] = 'A';
	symbols[symbol_count++] = 'B';
	symbols[symbol_count++] = 'C';
	symbols[symbol_count++] = 'D';
	symbols[symbol_count++] = 'E';
	symbols[symbol_count++] = 'F';
	symbols[symbol_count++] = 'G';
	symbols[symbol_count++] = 'H';
	symbols[symbol_count++] = 'I';
	symbols[symbol_count++] = 'J';
	symbols[symbol_count++] = 'K';
	symbols[symbol_count++] = 'L';
	symbols[symbol_count++] = 'M';
	symbols[symbol_count++] = 'N';
	symbols[symbol_count++] = 'O';
	symbols[symbol_count++] = 'P';
	symbols[symbol_count++] = 'Q';
	symbols[symbol_count++] = 'R';
	symbols[symbol_count++] = 'S';
	symbols[symbol_count++] = 'T';
	symbols[symbol_count++] = 'U';
	symbols[symbol_count++] = 'V';
	symbols[symbol_count++] = 'W';
	symbols[symbol_count++] = 'X';
	symbols[symbol_count++] = 'Y';
	symbols[symbol_count++] = 'Z';
	symbols[symbol_count++] = '@';
	symbols[symbol_count++] = '6';
	symbols[symbol_count++] = '\'';
	symbols[symbol_count++] = '&';
	symbols[symbol_count++] = ';';
	symbols[symbol_count++] = ':';
	symbols[symbol_count++] = '~';
}

/* methods */
int MonsterAnalyzer::parseMessages(string *messages) {
	if (messages->find(MO_REALLY_ATTACK, 0) != string::npos) {
		action = YES;
		return 100;
	}
	return 0;
}

int MonsterAnalyzer::analyze(int row, int col, unsigned char symbol) {
	cerr << "found monster at " << row << ", " << col << " - " << symbol << endl;
	for (int m = 0; m < MO_MAX_MONSTERS; ++m) {
		if (monsters[m].symbol == NOMONSTER || (monsters[m].symbol == symbol && monsters[m].last_seen != saiph->world->player.turn)) {
			if (monsters[m].symbol == NOMONSTER)
				cerr << "this seems to be a new monster" << endl;
			else
				cerr << "seems to be a known monster. old pos: " << monsters[m].row << ", " << monsters[m].col << endl;
			/* unblock path for pathing algorithm */
			if (monsters[m].row != -1 && monsters[m].col != -1 && saiph->branches[saiph->current_branch]->unpassable[saiph->world->player.dungeon][monsters[m].row][monsters[m].col] == 2)
				saiph->branches[saiph->current_branch]->unpassable[saiph->world->player.dungeon][monsters[m].row][monsters[m].col] = 0;
			monsters[m].row = row;
			monsters[m].col = col;
			monsters[m].symbol = symbol;
			monsters[m].last_seen = saiph->world->player.turn;
			/* block path for pathing algorithm */
			if (saiph->branches[saiph->current_branch]->unpassable[saiph->world->player.dungeon][row][col] == 0)
				saiph->branches[saiph->current_branch]->unpassable[saiph->world->player.dungeon][row][col] = 2;
			return 0;
		}
	}
	/* we're tracking too many monsters, return */
	return 0;
}

int MonsterAnalyzer::finish() {
	/* if engulfed try to fight our way out */
	if (saiph->engulfed) {
		action = MOVE_NW; // doesn't matter which direction
		return 70;
	}
	/* fight nearest monster */
	int shortest_distance = -1;
	int m = NOMONSTER;
	unsigned char best_move = ILLEGAL_MOVE;
	int distance = -1;
	bool direct_line = false;
	unsigned char move = ILLEGAL_MOVE;
	for (int mc = 0; mc < MO_MAX_MONSTERS; ++mc) {
		if (monsters[mc].symbol == NOMONSTER)
			continue;
		move = saiph->shortestPath(monsters[mc].row, monsters[mc].col, true, distance, direct_line);
		if (move == ILLEGAL_MOVE || (distance <= 1 && saiph->world->map[monsters[mc].row][monsters[mc].col] != monsters[mc].symbol)) {
			/* can't find monster, forget it */
			cerr << "unable to find monster " << monsters[mc].symbol << ". monster forgotten" << endl;
			monsters[mc].symbol = NOMONSTER;
			/* unblock path for pathing algorithm */
			if (saiph->branches[saiph->current_branch]->unpassable[saiph->world->player.dungeon][monsters[mc].row][monsters[mc].col] == 2)
				saiph->branches[saiph->current_branch]->unpassable[saiph->world->player.dungeon][monsters[mc].row][monsters[mc].col] = 0;
			continue;
		}
		if (m == NOMONSTER || distance < shortest_distance) {
			if (monsters[mc].symbol == 'e' || monsters[mc].symbol == '@')
				shortest_distance = 666; // hack, we don't want to attack these monsters unless we have to
			else
				shortest_distance = distance;
			m = mc;
			best_move = move;
		}
	}
	if (m != NOMONSTER) {
		cerr << "fighting " << monsters[m].symbol << endl;
		action = best_move;
		if (monsters[m].symbol == 'e' || monsters[m].symbol == '@')
			return 1; // don't fight these unless there's no way out
		else
			return 70;
	}
	return 0;
}

void MonsterAnalyzer::command(string *command) {
	command->push_back(action);
}
