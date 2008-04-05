#include "MonsterAnalyzer.h"

/* constructors */
MonsterAnalyzer::MonsterAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	for (int m = 0; m < MO_MAX_MONSTERS; ++m) {
		monsters[m].row = -1;
		monsters[m].col = -1;
		monsters[m].symbol = -1;
		monsters[m].last_seen = -1;
	}
	symbols[symbol_count++] = MONSTER_a;
	symbols[symbol_count++] = MONSTER_b;
	symbols[symbol_count++] = MONSTER_c;
	symbols[symbol_count++] = MONSTER_d;
	symbols[symbol_count++] = MONSTER_e;
	symbols[symbol_count++] = MONSTER_f;
	symbols[symbol_count++] = MONSTER_g;
	symbols[symbol_count++] = MONSTER_h;
	symbols[symbol_count++] = MONSTER_i;
	symbols[symbol_count++] = MONSTER_j;
	symbols[symbol_count++] = MONSTER_k;
	symbols[symbol_count++] = MONSTER_l;
	symbols[symbol_count++] = MONSTER_m;
	symbols[symbol_count++] = MONSTER_n;
	symbols[symbol_count++] = MONSTER_o;
	symbols[symbol_count++] = MONSTER_p;
	symbols[symbol_count++] = MONSTER_q;
	symbols[symbol_count++] = MONSTER_r;
	symbols[symbol_count++] = MONSTER_s;
	symbols[symbol_count++] = MONSTER_t;
	symbols[symbol_count++] = MONSTER_u;
	symbols[symbol_count++] = MONSTER_v;
	symbols[symbol_count++] = MONSTER_w;
	symbols[symbol_count++] = MONSTER_x;
	symbols[symbol_count++] = MONSTER_y;
	symbols[symbol_count++] = MONSTER_z;
	symbols[symbol_count++] = MONSTER_A;
	symbols[symbol_count++] = MONSTER_B;
	symbols[symbol_count++] = MONSTER_C;
	symbols[symbol_count++] = MONSTER_D;
	symbols[symbol_count++] = MONSTER_E;
	symbols[symbol_count++] = MONSTER_F;
	symbols[symbol_count++] = MONSTER_G;
	symbols[symbol_count++] = MONSTER_H;
	symbols[symbol_count++] = MONSTER_I;
	symbols[symbol_count++] = MONSTER_J;
	symbols[symbol_count++] = MONSTER_K;
	symbols[symbol_count++] = MONSTER_L;
	symbols[symbol_count++] = MONSTER_M;
	symbols[symbol_count++] = MONSTER_N;
	symbols[symbol_count++] = MONSTER_O;
	symbols[symbol_count++] = MONSTER_P;
	symbols[symbol_count++] = MONSTER_Q;
	symbols[symbol_count++] = MONSTER_R;
	symbols[symbol_count++] = MONSTER_S;
	symbols[symbol_count++] = MONSTER_T;
	symbols[symbol_count++] = MONSTER_U;
	symbols[symbol_count++] = MONSTER_V;
	symbols[symbol_count++] = MONSTER_W;
	symbols[symbol_count++] = MONSTER_X;
	symbols[symbol_count++] = MONSTER_Y;
	symbols[symbol_count++] = MONSTER_Z;
	symbols[symbol_count++] = MONSTER_AT;
	symbols[symbol_count++] = MONSTER_6;
	symbols[symbol_count++] = MONSTER_APOSTROPHE;
	symbols[symbol_count++] = MONSTER_AND;
	symbols[symbol_count++] = MONSTER_SEMICOLON;
	symbols[symbol_count++] = MONSTER_COLON;
	symbols[symbol_count++] = MONSTER_TILDE;
}

/* methods */
void MonsterAnalyzer::analyze(int row, int col, char symbol) {
	cerr << "found monster at " << row << ", " << col << " - " << symbol << endl;
	for (int m = 0; m < MO_MAX_MONSTERS; ++m) {
		if (monsters[m].symbol == -1 || (monsters[m].symbol == symbol && monsters[m].last_seen != saiph->world->player.turn)) {
			if (monsters[m].symbol == -1)
				cerr << "this seems to be a new monster" << endl;
			else
				cerr << "seems to be a known monster. old pos: " << monsters[m].row << ", " << monsters[m].col << endl;
			monsters[m].row = row;
			monsters[m].col = col;
			monsters[m].symbol = symbol;
			monsters[m].last_seen = saiph->world->player.turn;
			return;
		}
	}
	/* we're tracking too many monsters, return */
	return;
}

void MonsterAnalyzer::finish() {
	/* figure out which monster to attack */
	for (int mc = 0; mc < MO_MAX_MONSTERS; ++mc) {
		if (monsters[mc].symbol == -1)
			continue;
		/* fight nearest monster */
		int shortest_distance = 666;
		int m = -1;
		int best_move = -1;
		int distance = 0;
		bool direct_line = false;
		char move = -2;
		for (int mc2 = 0; mc2 < MO_MAX_MONSTERS; ++mc2) {
			if (monsters[mc2].symbol == -1)
				continue;
			move = saiph->shortestPath(monsters[mc2].row, monsters[mc2].col, true, distance, direct_line);
			if (move == -1 || (distance <= 1 && saiph->world->map[monsters[mc2].row][monsters[mc2].col] != monsters[mc2].symbol)) {
				/* can't find monster, forget it */
				cerr << "unable to find monster " << monsters[mc2].symbol << ". monster forgotten" << endl;
				monsters[mc2].symbol = -1;
				continue;
			}
			if (distance < shortest_distance) {
				shortest_distance = distance;
				m = mc2;
				best_move = move;
			}
		}
		if (m != -1) {
			cerr << "fighting " << monsters[m].symbol << endl;
			char command[2];
			command[0] = best_move;
			command[1] = '\0';
			saiph->setNextCommand(command, 70);
			break;
		}
	}
}
