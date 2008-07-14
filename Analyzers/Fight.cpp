#include "Fight.h"

/* constructors */
Fight::Fight(Saiph *saiph) : saiph(saiph), action("") {
}

/* methods */
void Fight::command(string *command) {
	*command = action;
}

int Fight::finish() {
	action = "";
	/* if engulfed try to fight our way out */
	if (saiph->engulfed) {
		action = MOVE_NW; // doesn't matter which direction
		return FIGHT_ATTACK_MONSTER;
	}
	/* fight nearest monster */
	int b = saiph->current_branch;
	int l = saiph->current_level;
	int best_distance = INT_MAX;
	unsigned char best_move = ILLEGAL_MOVE;
	unsigned char best_symbol = ILLEGAL_MONSTER;
	int best_color = NO_COLOR;
	for (list<Monster>::iterator m = saiph->monstertracker->monsters[b][l].begin(); m!= saiph->monstertracker->monsters[b][l].end(); ++m) {
		if (m->symbol == PET)
			continue; // we're not fighting pets :)
		int distance = -1;
		bool straight_line = false;
		unsigned char move = saiph->shortestPath(*m, true, &distance, &straight_line);
		if (move == ILLEGAL_MOVE)
			continue; // can't path to this monster
		if (distance >= best_distance)
			continue; // currently we'll fight the nearest monster
		best_distance = distance;
		best_move = move;
		best_symbol = m->symbol;
		best_color = m->color;
	}
	if (best_move == ILLEGAL_MOVE)
		return 0; // nothing to fight
	action = best_move;
	if (best_symbol == 'e' && best_color == BLUE)
		return FIGHT_ATTACK_BLUE_E;
	else if (best_symbol == '@' && best_color == WHITE)
		return FIGHT_ATTACK_WHITE_AT;
	return FIGHT_ATTACK_MONSTER;
}

int Fight::parseMessages(string *messages) {
	if (messages->find(FIGHT_REALLY_ATTACK, 0) != string::npos) {
		action = YES;
		return FIGHT_CONTINUE_ACTION;
	}
	return 0;
}
