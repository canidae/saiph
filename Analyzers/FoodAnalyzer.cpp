#include "FoodAnalyzer.h"

/* constructors */
FoodAnalyzer::FoodAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	action = FO_DO_NOTHING;
	action_move = -1;
	symbols[symbol_count++] = FOOD;
}

/* methods */
int FoodAnalyzer::parseMessages(string *messages) {
	//if (messages->find(FO_ENGRAVE_WITH, 0) != string::npos) {
	//}
	return 0;
}

int FoodAnalyzer::analyze(int row, int col, char symbol) {
	for (list<FO_Food>::iterator f = food.begin(); f != food.end(); ++f) {
		if (f->row == row && f->col == col)
			return 0;
	}
	/* new food */
	FO_Food f;
	f.row = row;
	f.col = col;
	//f.rots_turn = ?
	food.push_back(f);
	return 0;
}

int FoodAnalyzer::finish() {
	/* if we're [over]satiated then do nothing */
	if (saiph->world->player.hunger == SATIATED || saiph->world->player.hunger == OVERSATIATED)
		return 0;
	/* eat corpse unless it's too old */
	int nearest = 666;
	char best_move = -1;
	list<FO_Food>::iterator f = food.begin();
	for (list<FO_Food>::iterator f = food.begin(); f != food.end(); ++f) {
		if (saiph->world->map[f->row][f->col] != FOOD) {
			/* food is gone, remove from list.
			 * must do f-- or the iterator will skip an entry */
			food.erase(f--);
			continue;
		}
		int distance = -1;
		bool direct_line = false;
		char move = saiph->shortestPath(f->row, f->col, false, distance, direct_line);
		distance += saiph->world->player.turn - f->rots_turn;
		if (distance > 0) {
			/* it's gonna rot before we get there (most likely) */
			continue;
		}
		if (distance < nearest) {
			nearest = distance;
			best_move = move;
		}
	}
	if (best_move != -1) {
		action = FO_EAT_CORPSE;
		action_move = best_move;
	}

	/* eat food ration/fruits/etc when we're weak */
	//if (saiph->world->player.hunger == WEAK) {
	//}

	/* pray when we're fainting */
	/*
	if (saiph->world->player.hunger == FAINTING) {
		action = FO_PRAY;
		return 100;
	}
	*/
	return 0;
}

void FoodAnalyzer::command() {
	switch (action) {
		case FO_EAT_CORPSE:
			break;

		default:
			cerr << "FoodAnalyzer don't know what to do" << endl;
			exit(1);
			break;
	}
}
