#include "FightAnalyzer.h"

/* constructors */
FightAnalyzer::FightAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	monster_count = 0;
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
void FightAnalyzer::analyze(int row, int col, char symbol) {
	cerr << "Found monster at " << row << ", " << col << " - " << symbol << endl;
	if (monster_count >= FI_MAX_MONSTERS)
		return; // tracking too many monsters
	monsters[monster_count].row = row;
	monsters[monster_count].col = col;
	monsters[monster_count].threat = (int) symbol;
	monsters[monster_count].no_melee = (symbol == 'e') ? true : false;
	++monster_count;
}

void FightAnalyzer::finish() {
	/* figure out which monster to attack */
	for (int mc = 0; mc < monster_count; ++mc) {
		int toughest = -1;
		int threat = -1;
		for (int m = 0; m < monster_count; ++m) {
			if (monsters[m].threat > threat) {
				toughest = m;
				threat = monsters[m].threat;
			}
		}
		if (toughest == -1) {
			/* no threatening monster */
			break;
		}
		cerr << "Fighting " << (char) monsters[toughest].threat << endl;
		int distance = 0;
		bool direct_line = false;
		char move = saiph->shortestPath(saiph->current_branch, saiph->world->player.status.dungeon, monsters[toughest].row, monsters[toughest].col, distance, direct_line);
		if (move != -1) {
			char command[2];
			command[0] = move;
			command[1] = '\0';
			saiph->setNextCommand(command, 70);
			break;
		} else {
			monsters[toughest].threat = -1;
		}
	}
	monster_count = 0;
}
