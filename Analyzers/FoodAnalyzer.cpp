#include "FoodAnalyzer.h"

/* constructors */
FoodAnalyzer::FoodAnalyzer(Saiph *saiph) {
	this->saiph = saiph;
	action = FO_DO_NOTHING;
	action_move = ILLEGAL_MOVE;
	/* inedible (corpses that give stuff we don't want; hallu, stun, mimic, lycantrophy, petrify, ...) */
	inedible.push_back("abbot");
	inedible.push_back("bat");
	inedible.push_back("chameleon");
	inedible.push_back("chickatrice");
	inedible.push_back("cockatrice");
	inedible.push_back("Death");
	inedible.push_back("dog");
	inedible.push_back("doppelganger");
	inedible.push_back("Famine");
	inedible.push_back("giant bat");
	inedible.push_back("giant mimic");
	inedible.push_back("green slime");
	inedible.push_back("housecat");
	inedible.push_back("kitten");
	inedible.push_back("large cat");
	inedible.push_back("large dog");
	inedible.push_back("large mimic");
	inedible.push_back("little dog");
	inedible.push_back("Medusa");
	inedible.push_back("Pestilence");
	inedible.push_back("small mimic");
	inedible.push_back("werejackal");
	inedible.push_back("wererat");
	inedible.push_back("werewolf");
	/* symbols we'll analyze */
	vector<unsigned char> symbols;
	symbols.push_back(FOOD);
	saiph->registerAnalyzerSymbols(this, symbols);
}

/* methods */
int FoodAnalyzer::parseMessages(string *messages) {
	//if (messages->find(FO_ENGRAVE_WITH, 0) != string::npos) {
	//}
	return 0;
}

int FoodAnalyzer::analyze(int row, int col, unsigned char symbol) {
	for (list<FO_Food>::iterator f = food.begin(); f != food.end(); ++f) {
		if (f->row == row && f->col == col)
			return 0;
	}
	/* new food */
	int rots_turn = 1;
	for (list<Dungeon>::iterator d = saiph->history->begin(); d != saiph->history->end(); ) {
		if (d->map[row][col] == 'M' || d->map[row][col] == 'Z') {
			/* mummy/zombie, rotten */
			rots_turn = 1;
			break;
		}
		++d;
	}
	FO_Food f;
	f.row = row;
	f.col = col;
	f.rots_turn = rots_turn;
	food.push_back(f);
	return 0;
}

int FoodAnalyzer::finish() {
	/* if we're [over]satiated then do nothing */
	if (saiph->world->player.hunger == SATIATED || saiph->world->player.hunger == OVERSATIATED)
		return 0;
	/* eat corpse unless it's too old */
	int nearest = 666;
	unsigned char best_move = ILLEGAL_MOVE;
	list<FO_Food>::iterator f = food.begin();
	for (list<FO_Food>::iterator f = food.begin(); f != food.end(); ) {
		if (saiph->world->map[f->row][f->col] != FOOD) {
			/* food is gone, remove from list */
			food.erase(f++);
			continue;
		}
		int distance = -1;
		bool direct_line = false;
		unsigned char move = saiph->shortestPath(f->row, f->col, false, distance, direct_line);
		distance += saiph->world->player.turn - f->rots_turn;
		++f;
		if (distance > 0) {
			/* it's gonna rot before we get there (most likely) */
			continue;
		}
		if (distance < nearest) {
			nearest = distance;
			best_move = move;
		}
	}
	if (best_move != ILLEGAL_MOVE) {
		action = FO_EAT_CORPSE;
		action_move = best_move;
		return 62;
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

void FoodAnalyzer::command(string *command) {
	switch (action) {
		case FO_EAT_CORPSE:
			break;

		default:
			cerr << "FoodAnalyzer don't know what to do" << endl;
			exit(1);
			break;
	}
}
