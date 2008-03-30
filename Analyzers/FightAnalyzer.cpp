#include "FightAnalyzer.h"

/* constructors */
FightAnalyzer::FightAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	this->type = ANALYZE_MONSTER;
	this->monster_count = 0;
}

/* destructors */
FightAnalyzer::~FightAnalyzer() {
}

/* methods */
void FightAnalyzer::analyze(int row, int col, char symbol) {
	cerr << "Found monster at " << row << ", " << col << " - " << symbol << endl;
	if (monster_count >= FA_MAX_MONSTERS)
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
			monster_count = 0;
			return;
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
			monster_count = 0;
			return;
		} else {
			monsters[toughest].threat = -1;
		}
	}
}
