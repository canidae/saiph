#include "LootAnalyzer.h"

/* constructors */
LootAnalyzer::LootAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
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
	//symbols[symbol_count++] = STATUE;
	//symbols[symbol_count++] = IRON_BALL;
	//symbols[symbol_count++] = CHAINS;
	//symbols[symbol_count++] = VENOM;
}

/* methods */
int LootAnalyzer::parseMessages(string *messages) {
	//} else if (messages->find(DO_KICK_OPEN, 0) != string::npos) {
	return 0;
}

int LootAnalyzer::analyze(int row, int col, char symbol) {
	return 0;
}

int LootAnalyzer::finish() {
	return 0;
}

void LootAnalyzer::command() {
	char command[10];
	command[0] = '#';
	command[1] = 'e';
	command[2] = 'n';
	command[3] = 'h';
	command[4] = 'a';
	command[5] = 'n';
	command[6] = 'c';
	command[7] = 'e';
	command[8] = '\n';
	command[9] = '\0';
	saiph->world->command(command);
}
