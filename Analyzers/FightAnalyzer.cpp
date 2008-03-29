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
	if (monster_count <= 0)
		return;
	int toughest = 0;
	int threat = 0;
	for (int m = 0; m < monster_count; ++m) {
		if (monsters[m].threat > threat) {
			threat = monsters[m].threat;
			toughest = m;
		}
	}
	char move = saiph->shortestPath(monsters[toughest].row, monsters[toughest].col);
	if (move == -1) {
		cerr << "Unable to find path to monster" << endl;
		return;
	}
	char command[2];
	command[0] = move;
	command[1] = '\0';
	saiph->setNextCommand(command, 60);
	/* reset */
	monster_count = 0;
}
