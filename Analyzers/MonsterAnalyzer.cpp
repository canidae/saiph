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
	vector<unsigned char> symbols;
	symbols.push_back('a');
	symbols.push_back('b');
	symbols.push_back('c');
	symbols.push_back('d');
	symbols.push_back('e');
	symbols.push_back('f');
	symbols.push_back('g');
	symbols.push_back('h');
	symbols.push_back('i');
	symbols.push_back('j');
	symbols.push_back('k');
	symbols.push_back('l');
	symbols.push_back('m');
	symbols.push_back('n');
	symbols.push_back('o');
	symbols.push_back('p');
	symbols.push_back('q');
	symbols.push_back('r');
	symbols.push_back('s');
	symbols.push_back('t');
	symbols.push_back('u');
	symbols.push_back('v');
	symbols.push_back('w');
	symbols.push_back('x');
	symbols.push_back('y');
	symbols.push_back('z');
	symbols.push_back('A');
	symbols.push_back('B');
	symbols.push_back('C');
	symbols.push_back('D');
	symbols.push_back('E');
	symbols.push_back('F');
	symbols.push_back('G');
	symbols.push_back('H');
	symbols.push_back('I');
	symbols.push_back('J');
	symbols.push_back('K');
	symbols.push_back('L');
	symbols.push_back('M');
	symbols.push_back('N');
	symbols.push_back('O');
	symbols.push_back('P');
	symbols.push_back('Q');
	symbols.push_back('R');
	symbols.push_back('S');
	symbols.push_back('T');
	symbols.push_back('U');
	symbols.push_back('V');
	symbols.push_back('W');
	symbols.push_back('X');
	symbols.push_back('Y');
	symbols.push_back('Z');
	symbols.push_back('@');
	symbols.push_back('6');
	symbols.push_back('\'');
	symbols.push_back('&');
	symbols.push_back(';');
	symbols.push_back(':');
	symbols.push_back('~');
	saiph->registerAnalyzerSymbols(this, symbols);
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
			saiph->map[saiph->current_branch][saiph->current_level].monster[monsters[m].row][monsters[m].col] = NOMONSTER;
			monsters[m].row = row;
			monsters[m].col = col;
			monsters[m].symbol = symbol;
			monsters[m].last_seen = saiph->world->player.turn;
			/* block path for pathing algorithm */
			saiph->map[saiph->current_branch][saiph->current_level].monster[row][col] = symbol;
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
	int m = -1;
	unsigned char best_move = ILLEGAL_MOVE;
	int distance = -1;
	bool direct_line = false;
	unsigned char move = ILLEGAL_MOVE;
	for (int mc = 0; mc < MO_MAX_MONSTERS; ++mc) {
		if (monsters[mc].symbol == NOMONSTER)
			continue;
		Point tmp;
		tmp.row = monsters[mc].row;
		tmp.col = monsters[mc].col;
		move = saiph->shortestPath(tmp, true, &distance, &direct_line);
		if (move == ILLEGAL_MOVE || (distance <= 1 && saiph->world->map[monsters[mc].row][monsters[mc].col] != monsters[mc].symbol)) {
			/* can't find monster, forget it */
			cerr << "unable to find monster " << monsters[mc].symbol << ". monster forgotten" << endl;
			monsters[mc].symbol = NOMONSTER;
			/* unblock path for pathing algorithm */
			saiph->map[saiph->current_branch][saiph->current_level].monster[monsters[mc].row][monsters[mc].col] = NOMONSTER;
			continue;
		}
		if (m == -1 || distance < shortest_distance) {
			if (monsters[mc].symbol == 'e' || monsters[mc].symbol == '@')
				shortest_distance = 666; // hack, we don't want to attack these monsters unless we have to
			else
				shortest_distance = distance;
			m = mc;
			best_move = move;
		}
	}
	if (m != -1) {
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
