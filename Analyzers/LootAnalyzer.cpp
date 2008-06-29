#include "LootAnalyzer.h"

/* constructors */
LootAnalyzer::LootAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	action = LO_NOTHING;
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
	if (messages->find(LO_THINGS_HERE, 0) != string::npos) {
		/* multiple items here */
		action = LO_LOOT;
		return 100;
	} else if (messages->find(LO_YOU_SEE, 0) != string::npos) {
		/* single item here */
		action = LO_LOOT;
		return 100;
	} else if (messages->find(LO_PICK_UP, 0) != string::npos) {
		/* pick up menu */
		action = LO_SELECT_ALL;
		return 100;
	} else if (messages->find(LO_LITTLE_LIFTING, 0) != string::npos) {
		action = LO_NO_LOOT;
		return 100;
	} else if (messages->find(LO_EXTREME_LIFTING, 0) != string::npos) {
		action = LO_NO_LOOT;
		return 100;
	} else if (messages->find(LO_MUCH_LIFTING, 0) != string::npos) {
		action = LO_NO_LOOT;
		return 100;
	}
	return 0;
}

int LootAnalyzer::analyze(int row, int col, unsigned char symbol) {
	return 0;
}

int LootAnalyzer::finish() {
	return 0;
}

void LootAnalyzer::command(string *command) {
	switch (action) {
		case LO_LOOT:
			command->push_back(',');
			break;

		case LO_SELECT_ALL:
			command->push_back(',');
			command->push_back(' ');
			break;

		case LO_NO_LOOT:
			command->push_back(NO);
			break;

		default:
			cerr << "LootAnalyzer don't know what to do" << endl;
			exit(1);
			break;
	}
}
